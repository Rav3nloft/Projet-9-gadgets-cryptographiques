#include <time.h>
#include "gadgets_conv.h"

#define N_ITER 1000

void BenchA2B(int d) {
    Share secret = 0xDEADBEEF;

    for (int i = 0; i < 100; i++) {
        ArithSharing partageArith = MasquerArith(secret, d);
        BoolSharing partageBool;
        A2B(&partageArith, &partageBool);
    }

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (int i = 0; i < N_ITER; i++) {
        ArithSharing partageArith = MasquerArith(secret, d);
        BoolSharing partageBool;
        A2B(&partageArith, &partageBool);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    long nsTotaux = (fin.tv_sec  - debut.tv_sec)  * 1000000000L
                  + (fin.tv_nsec - debut.tv_nsec);
    double nsMoyen = (double)nsTotaux / N_ITER;

    RandResetCounter();
    ArithSharing partageArith = MasquerArith(secret, d);
    RandResetCounter();
    BoolSharing partageBool;
    A2B(&partageArith, &partageBool);
    uint32_t nombreRandoms = RandGetCounter();

    printf("  d=%d | temps moy : %8.2f ns | randoms : %u\n",
           d, nsMoyen, nombreRandoms);
}

void BenchB2A_Classic(int d) {
    Share secret = 0xDEADBEEF;

    for (int i = 0; i < 100; i++) {
        BoolSharing partageBool = MasquerBool(secret, d);
        ArithSharing partageArith;
        B2A_Classic(&partageBool, &partageArith);
    }

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (int i = 0; i < N_ITER; i++) {
        BoolSharing partageBool = MasquerBool(secret, d);
        ArithSharing partageArith;
        B2A_Classic(&partageBool, &partageArith);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    long nsTotaux = (fin.tv_sec  - debut.tv_sec)  * 1000000000L
                  + (fin.tv_nsec - debut.tv_nsec);
    double nsMoyen = (double)nsTotaux / N_ITER;

    RandResetCounter();
    BoolSharing partageBool = MasquerBool(secret, d);
    RandResetCounter();
    ArithSharing partageArith;
    B2A_Classic(&partageBool, &partageArith);
    uint32_t nombreRandoms = RandGetCounter();

    printf("  d=%d | temps moy : %8.2f ns | randoms : %u\n",
           d, nsMoyen, nombreRandoms);
}

void BenchB2X2A(int d) {
    Share secret = 0xDEADBEEF;

    for (int i = 0; i < 100; i++) {
        BoolSharing partageBool = MasquerBool(secret, d);
        ArithSharing partageArith;
        B2X2A(&partageBool, &partageArith);
    }

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (int i = 0; i < N_ITER; i++) {
        BoolSharing partageBool = MasquerBool(secret, d);
        ArithSharing partageArith;
        B2X2A(&partageBool, &partageArith);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    long nsTotaux = (fin.tv_sec  - debut.tv_sec)  * 1000000000L
                  + (fin.tv_nsec - debut.tv_nsec);
    double nsMoyen = (double)nsTotaux / N_ITER;

    RandResetCounter();
    BoolSharing partageBool = MasquerBool(secret, d);
    RandResetCounter();
    ArithSharing partageArith;
    B2X2A(&partageBool, &partageArith);
    uint32_t nombreRandoms = RandGetCounter();

    printf("  d=%d | temps moy : %8.2f ns | randoms : %u\n",
           d, nsMoyen, nombreRandoms);
}

int main(void) {
    printf("___ Benchmark Gadgets Conv (%d iterations) ___\n\n", N_ITER);
    RandInit(67676767);

    int ordres[] = {1, 2, 3, 4};
    int nombreOrdres = 4;

    printf("--- A2B ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        BenchA2B(ordres[i]);

    printf("\n--- B2A_Classic ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        BenchB2A_Classic(ordres[i]);

    printf("\n--- B2X2A ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        BenchB2X2A(ordres[i]);

    return 0;
}
