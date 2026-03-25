#include "gadgets_basic.h"

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

void TestSecXOR(void) {
    printf("\n--- TestSecXOR ---\n");

    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;
    Share attendu = a ^ b;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageA = MasquerBool(a, d);
        BoolSharing partageB = MasquerBool(b, d);
        BoolSharing partageResultat;
        SecXOR(&partageA, &partageB, &partageResultat);
        Share resultat = Demasquer(&partageResultat);

        char msg[64];
        sprintf(msg, "0x%08X ^ 0x%08X = 0x%08X (d=%d)", a, b, attendu, d);
        Check(msg, resultat == attendu);
    }
}

void TestSecNOT(void) {
    printf("\n--- TestSecNOT ---\n");

    Share a = 0xDEADBEEF;
    Share attendu = ~a;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageA = MasquerBool(a, d);
        BoolSharing partageResultat;
        SecNOT(&partageA, &partageResultat);
        Share resultat = Demasquer(&partageResultat);

        char msg[64];
        sprintf(msg, "~0x%08X = 0x%08X (d=%d)", a, attendu, d);
        Check(msg, resultat == attendu);
    }
}

int main(void) {
    printf("___ Tests Gadgets Basic ___\n");
    RandInit(67676767);

    TestSecXOR();
    TestSecNOT();

    printf("\n--- Résultat : %d OK, %d FAIL ---\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
