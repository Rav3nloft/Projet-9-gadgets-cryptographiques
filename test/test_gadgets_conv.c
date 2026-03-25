#include "gadgets_conv.h"

int passed = 0;
int failed = 0;

static void Check(char *testName, int condition) {
    if (condition) {
        printf("  [OK]   %s\n", testName);
        passed++;
    } else {
        printf("  [FAIL] %s\n", testName);
        failed++;
    }
}

void TestSecADD(void) {
    printf("\n--- TestSecADD ---\n");

    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;
    Share attendu = a + b;

    for (int d = 1; d <= 4; d++) {
        BoolSharing partageA = MasquerBool(a, d);
        BoolSharing partageB = MasquerBool(b, d);
        BoolSharing partageResultat;
        SecADD(&partageA, &partageB, &partageResultat);
        Share resultat = Demasquer(&partageResultat);

        char msg[64];
        sprintf(msg, "0x%08X + 0x%08X = 0x%08X (d=%d)", a, b, attendu, d);
        Check(msg, resultat == attendu);
    }
}

void TestA2B(void) {
    printf("\n--- TestA2B ---\n");

    Share secret = 0xDEADBEEF;

    for (int d = 1; d <= 4; d++) {
        ArithSharing partageArith = MasquerArith(secret, d);
        BoolSharing partageBool;
        A2B(&partageArith, &partageBool);
        Share resultat = Demasquer(&partageBool);

        char msg[64];
        sprintf(msg, "A2B(0x%08X) -> 0x%08X (d=%d)", secret, resultat, d);
        Check(msg, resultat == secret);
    }
}

void TestB2A_Classic(void) {
    printf("\n--- TestB2A_Classic ---\n");

    Share secret = 0xDEADBEEF;

    for (int d = 1; d <= 4; d++) {
        BoolSharing partageBool = MasquerBool(secret, d);
        ArithSharing partageArith;
        B2A_Classic(&partageBool, &partageArith);
        Share resultat = DemasquerArith(&partageArith);

        char msg[64];
        sprintf(msg, "B2A_Classic(0x%08X) -> 0x%08X (d=%d)", secret, resultat, d);
        Check(msg, resultat == secret);
    }
}

void TestB2X2A(void) {
    printf("\n--- TestB2X2A ---\n");

    Share secret = 0xDEADBEEF;

    for (int d = 1; d <= 4; d++) {
        BoolSharing partageBool = MasquerBool(secret, d);
        ArithSharing partageArith;
        B2X2A(&partageBool, &partageArith);
        Share resultat = DemasquerArith(&partageArith);

        char msg[64];
        sprintf(msg, "B2X2A(0x%08X) -> 0x%08X (d=%d)", secret, resultat, d);
        Check(msg, resultat == secret);
    }
}

void TestRoundtrip(void) {
    printf("\n--- TestRoundtrip ---\n");

    Share secret = 0xDEADBEEF;

    for (int d = 1; d <= 4; d++) {
        ArithSharing partageArith = MasquerArith(secret, d);
        BoolSharing partageBool;
        A2B(&partageArith, &partageBool);
        ArithSharing partageArithFinal;
        B2A_Classic(&partageBool, &partageArithFinal);
        Share resultat = DemasquerArith(&partageArithFinal);

        char msg[64];
        sprintf(msg, "A2B puis B2A_Classic(0x%08X) -> 0x%08X (d=%d)",
                secret, resultat, d);
        Check(msg, resultat == secret);
    }

    for (int d = 1; d <= 4; d++) {
        ArithSharing partageArith = MasquerArith(secret, d);
        BoolSharing partageBool;
        A2B(&partageArith, &partageBool);
        ArithSharing partageArithFinal;
        B2X2A(&partageBool, &partageArithFinal);
        Share resultat = DemasquerArith(&partageArithFinal);

        char msg[64];
        sprintf(msg, "A2B puis B2X2A(0x%08X) -> 0x%08X (d=%d)",
                secret, resultat, d);
        Check(msg, resultat == secret);
    }
}

int main(void) {
    printf("___ Tests Gadgets Conv ___\n");
    RandInit(67676767);

    TestSecADD();
    TestA2B();
    TestB2A_Classic();
    TestB2X2A();
    TestRoundtrip();

    printf("\n--- Résultat : %d OK, %d FAIL ---\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
