#include "rng.h"

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

void TestDeterminisme(void) {
    printf("\n--- TestDeterminisme ---\n");

    RandInit(67676767);
    Share a = RandShare();

    RandInit(67676767);
    Share b = RandShare();

    char msg[64];
    sprintf(msg, "graine 67676767 -> 0x%08X == 0x%08X", a, b);
    Check(msg, a == b);
}

void TestVariabilite(void) {
    printf("\n--- TestVariabilite ---\n");

    RandInit(67676767);
    Share a = RandShare();
    Share b = RandShare();

    char msg[64];
    sprintf(msg, "0x%08X != 0x%08X (valeurs consecutives)", a, b);
    Check(msg, a != b);
}

void TestRandZeroSharing(void) {
    printf("\n--- TestRandZeroSharing ---\n");

    RandInit(67676767);

    for (int d = 1; d <= MAX_ORDER; d++) {
        Share r[MAX_SHARES];
        RandZeroSharing(r, d);

        Share accumulateur = 0;
        for (int i = 0; i <= d; i++)
            accumulateur ^= r[i];

        char msg[64];
        sprintf(msg, "XOR(r[0..%d]) = 0x%08X attendu 0x00000000 (d=%d)", d, accumulateur, d);
        Check(msg, accumulateur == 0);
    }
}

int main(void) {
    printf("___ Tests RNG ___\n");

    TestDeterminisme();
    TestVariabilite();
    TestRandZeroSharing();

    printf("\n--- Résultat : %d OK, %d FAIL ---\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
