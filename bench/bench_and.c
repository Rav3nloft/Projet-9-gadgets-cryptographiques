#include <time.h>
#include "gadgets_and.h"

#define N_ITER 10000

void BenchmarkISW(int d) {
    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;

    for (int i = 0; i < 100; i++) {
        BoolSharing partageA = MasquerISW(a, d);
        BoolSharing partageB = MasquerISW(b, d);
        BoolSharing partageResultat;
        ISW_AND(&partageA, &partageB, &partageResultat);
    }

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (int i = 0; i < N_ITER; i++) {
        BoolSharing partageA = MasquerISW(a, d);
        BoolSharing partageB = MasquerISW(b, d);
        BoolSharing partageResultat;
        ISW_AND(&partageA, &partageB, &partageResultat);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    long nsTotaux = (fin.tv_sec  - debut.tv_sec)  * 1000000000L
                  + (fin.tv_nsec - debut.tv_nsec);
    double nsMoyen = (double)nsTotaux / N_ITER;

    RandResetCounter();
    BoolSharing partageA = MasquerISW(a, d);
    BoolSharing partageB = MasquerISW(b, d);
    RandResetCounter();
    BoolSharing partageResultat;
    ISW_AND(&partageA, &partageB, &partageResultat);
    uint32_t nombreRandoms = RandGetCounter();

    printf("  d=%d | temps moy : %8.2f ns | randoms : %u\n",
           d, nsMoyen, nombreRandoms);
}

void BenchmarkTrichina(void) {
    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;

    for (int i = 0; i < 100; i++) {
        BoolSharing partageA = MasquerBool(a, 1);
        BoolSharing partageB = MasquerBool(b, 1);
        BoolSharing partageResultat;
        Trichina_AND(&partageA, &partageB, &partageResultat);
    }

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (int i = 0; i < N_ITER; i++) {
        BoolSharing partageA = MasquerBool(a, 1);
        BoolSharing partageB = MasquerBool(b, 1);
        BoolSharing partageResultat;
        Trichina_AND(&partageA, &partageB, &partageResultat);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    long nsTotaux = (fin.tv_sec  - debut.tv_sec)  * 1000000000L
                  + (fin.tv_nsec - debut.tv_nsec);
    double nsMoyen = (double)nsTotaux / N_ITER;

    RandResetCounter();
    BoolSharing partageA = MasquerBool(a, 1);
    BoolSharing partageB = MasquerBool(b, 1);
    RandResetCounter();
    BoolSharing partageResultat;
    Trichina_AND(&partageA, &partageB, &partageResultat);
    uint32_t nombreRandoms = RandGetCounter();

    printf("  d=1 | temps moy : %8.2f ns | randoms : %u\n",
           nsMoyen, nombreRandoms);
}

void BenchmarkDOM(int d) {
    Share a = 0xDEADBEEF;
    Share b = 0xCAFEBABE;

    for (int i = 0; i < 100; i++) {
        BoolSharing partageA = MasquerBool(a, d);
        BoolSharing partageB = MasquerBool(b, d);
        BoolSharing partageResultat;
        DOM_AND(&partageA, &partageB, &partageResultat);
    }

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (int i = 0; i < N_ITER; i++) {
        BoolSharing partageA = MasquerBool(a, d);
        BoolSharing partageB = MasquerBool(b, d);
        BoolSharing partageResultat;
        DOM_AND(&partageA, &partageB, &partageResultat);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    long nsTotaux = (fin.tv_sec  - debut.tv_sec)  * 1000000000L
                  + (fin.tv_nsec - debut.tv_nsec);
    double nsMoyen = (double)nsTotaux / N_ITER;

    RandResetCounter();
    BoolSharing partageA = MasquerBool(a, d);
    BoolSharing partageB = MasquerBool(b, d);
    RandResetCounter();
    BoolSharing partageResultat;
    DOM_AND(&partageA, &partageB, &partageResultat);
    uint32_t nombreRandoms = RandGetCounter();

    printf("  d=%d | temps moy : %8.2f ns | randoms : %u\n",
           d, nsMoyen, nombreRandoms);
}

int main(void) {
    printf("___ Benchmark Gadgets AND (%d iterations) ___\n\n", N_ITER);
    RandInit(67676767);

    int ordres[] = {1, 2, 3, 4};
    int nombreOrdres = 4;

    printf("--- ISW_AND ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        BenchmarkISW(ordres[i]);

    printf("\n--- Trichina_AND ---\n");
    BenchmarkTrichina();

    printf("\n--- DOM_AND ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        BenchmarkDOM(ordres[i]);

    return 0;
}
