#include "gadgets_refresh.h"

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

void TestSecREF(void) {
    printf("\n--- TestSecREF ---\n");

    Share secret = 0xDEADBEEF;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageEntree = MasquerBool(secret, d);
        BoolSharing partageSortie;
        SecREF(&partageEntree, &partageSortie);

        Share avant = Demasquer(&partageEntree);
        Share apres = Demasquer(&partageSortie);

        char msg[64];
        sprintf(msg, "secret 0x%08X -> refresh -> 0x%08X (d=%d)", avant, apres, d);
        Check(msg, apres == secret);
    }
}

void TestSecEXP(void) {
    printf("\n--- TestSecEXP ---\n");

    Share secret = 0xDEADBEEF;
    int d = 1;

    BoolSharing partageEntree = MasquerBool(secret, d);
    BoolSharing partageSortie;
    SecEXP(&partageEntree, &partageSortie);

    Share apres = Demasquer(&partageSortie);

    char msg[128];
    sprintf(msg, "secret 0x%08X -> expand -> 0x%08X, shares %d -> %d",
            secret, apres, d + 1, partageSortie.d + 1);
    Check(msg, apres == secret && partageSortie.d == 2 * d + 1);
}

void TestRefreshMasks(void) {
    printf("\n--- TestRefreshMasks ---\n");

    Share secret = 0xDEADBEEF;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageEntree = MasquerBool(secret, d);
        BoolSharing partageSortie;
        RefreshMasks(&partageEntree, &partageSortie);

        Share apres = Demasquer(&partageSortie);

        char msg[64];
        sprintf(msg, "secret 0x%08X -> refreshmasks -> 0x%08X (d=%d)", secret, apres, d);
        Check(msg, apres == secret);
    }
}

void TestFullXOR(void) {
    printf("\n--- TestFullXOR ---\n");

    Share secret = 0xDEADBEEF;

    for (int d = 1; d <= 3; d++) {
        BoolSharing partageEntree = MasquerBool(secret, d);
        Share resultat;
        FullXOR(&partageEntree, &resultat);

        char msg[64];
        sprintf(msg, "secret 0x%08X -> fullxor -> 0x%08X (d=%d)", secret, resultat, d);
        Check(msg, resultat == secret);
    }
}

int main(void) {
    printf("___ Tests Gadgets Refresh ___\n");
    RandInit(67676767);

    TestSecREF();
    TestSecEXP();
    TestRefreshMasks();
    TestFullXOR();

    printf("\n--- Résultat : %d OK, %d FAIL ---\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
