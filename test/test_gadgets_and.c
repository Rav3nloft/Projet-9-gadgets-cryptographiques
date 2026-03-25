#include "gadgets_and.h"

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

void TestISW_AND(void) {
    printf("\n--- TestISW_AND ---\n");

    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;
    Share attendu = a & b;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageA = MasquerISW(a, d);
        BoolSharing partageB = MasquerISW(b, d);
        BoolSharing partageResultat;
        ISW_AND(&partageA, &partageB, &partageResultat);
        Share resultat = DemasquerISW(&partageResultat);

        char msg[64];
        sprintf(msg, "0x%08X & 0x%08X = 0x%08X (d=%d)", a, b, attendu, d);
        Check(msg, resultat == attendu);
    }
}

void TestTrichina_AND(void) {
    printf("\n--- TestTrichina_AND ---\n");

    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;
    Share attendu = a & b;

    BoolSharing partageA = MasquerBool(a, 1);
    BoolSharing partageB = MasquerBool(b, 1);
    BoolSharing partageResultat;
    Trichina_AND(&partageA, &partageB, &partageResultat);
    Share resultat = Demasquer(&partageResultat);

    char msg[64];
    sprintf(msg, "0x%08X & 0x%08X = 0x%08X (d=1)", a, b, attendu);
    Check(msg, resultat == attendu);
}

void TestDOM_AND(void) {
    printf("\n--- TestDOM_AND ---\n");

    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;
    Share attendu = a & b;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageA = MasquerBool(a, d);
        BoolSharing partageB = MasquerBool(b, d);
        BoolSharing partageResultat;
        DOM_AND(&partageA, &partageB, &partageResultat);
        Share resultat = Demasquer(&partageResultat);

        char msg[64];
        sprintf(msg, "0x%08X & 0x%08X = 0x%08X (d=%d)", a, b, attendu, d);
        Check(msg, resultat == attendu);
    }
}

void TestSecOR(void) {
    printf("\n--- TestSecOR ---\n");

    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;
    Share attendu = a | b;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageA = MasquerBool(a, d);
        BoolSharing partageB = MasquerBool(b, d);
        BoolSharing partageResultat;
        SecOR(&partageA, &partageB, &partageResultat);
        Share resultat = Demasquer(&partageResultat);

        char msg[64];
        sprintf(msg, "0x%08X | 0x%08X = 0x%08X (d=%d)", a, b, attendu, d);
        Check(msg, resultat == attendu);
    }
}

int main(void) {
    printf("___ Tests Gadgets AND ___\n");
    RandInit(67676767);

    TestISW_AND();
    TestTrichina_AND();
    TestDOM_AND();
    TestSecOR();

    printf("\n--- Résultat : %d OK, %d FAIL ---\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
