#include <math.h>
#include "gadgets_conv.h"

#define N_TRACES  10000
#define THRESHOLD 4.5

int HammingWeight(Share x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

double Leakage(BoolSharing *x) {
    double fuite = 0.0;
    for (int i = 0; i <= x->d; i++) {
        fuite += HammingWeight(x->s[i]);
    }
    return fuite;
}

double LeakageArith(ArithSharing *x) {
    double fuite = 0.0;
    for (int i = 0; i <= x->d; i++) {
        fuite += HammingWeight(x->s[i]);
    }
    return fuite;
}

double Welch(double *fixe, double *aleatoire, int n) {
    double moyenneFixe = 0.0, moyenneAleatoire = 0.0;
    for (int i = 0; i < n; i++) {
        moyenneFixe += fixe[i];
        moyenneAleatoire += aleatoire[i];
    }
    moyenneFixe /= n;
    moyenneAleatoire /= n;

    double varianceFixe = 0.0, varianceAleatoire = 0.0;
    for (int i = 0; i < n; i++) {
        varianceFixe += (fixe[i] - moyenneFixe) * (fixe[i] - moyenneFixe);
        varianceAleatoire += (aleatoire[i] - moyenneAleatoire) * (aleatoire[i] - moyenneAleatoire);
    }
    varianceFixe /= (n - 1);
    varianceAleatoire /= (n - 1);

    return (moyenneFixe - moyenneAleatoire) / sqrt(varianceFixe / n + varianceAleatoire / n);
}

void AfficherResultat(double t, int d) {
    int fuiteDetectee = fabs(t) > THRESHOLD;
    printf("  d=%d | t = %7.3f | %s\n",
           d, t, fuiteDetectee ? "[FUITE DETECTEE]" : "[OK] pas de fuite");
}

void TVLA_ISW(int d) {
    double *tracesFixe = malloc(N_TRACES * sizeof(double));
    double *tracesAleatoire = malloc(N_TRACES * sizeof(double));

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageA = MasquerISW(0xDEADBEEF, d);
        BoolSharing partageB = MasquerISW(0xCAFEBABE, d);
        BoolSharing partageResultat;
        ISW_AND(&partageA, &partageB, &partageResultat);
        tracesFixe[i] = Leakage(&partageResultat);
    }

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageA = MasquerISW(RandShare(), d);
        BoolSharing partageB = MasquerISW(RandShare(), d);
        BoolSharing partageResultat;
        ISW_AND(&partageA, &partageB, &partageResultat);
        tracesAleatoire[i] = Leakage(&partageResultat);
    }

    AfficherResultat(Welch(tracesFixe, tracesAleatoire, N_TRACES), d);

    free(tracesFixe);
    free(tracesAleatoire);
}

void TVLA_Trichina(void) {
    double *tracesFixe = malloc(N_TRACES * sizeof(double));
    double *tracesAleatoire = malloc(N_TRACES * sizeof(double));

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageA = MasquerBool(0xDEADBEEF, 1);
        BoolSharing partageB = MasquerBool(0xCAFEBABE, 1);
        BoolSharing partageResultat;
        Trichina_AND(&partageA, &partageB, &partageResultat);
        tracesFixe[i] = Leakage(&partageResultat);
    }

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageA = MasquerBool(RandShare(), 1);
        BoolSharing partageB = MasquerBool(RandShare(), 1);
        BoolSharing partageResultat;
        Trichina_AND(&partageA, &partageB, &partageResultat);
        tracesAleatoire[i] = Leakage(&partageResultat);
    }

    int fuiteDetectee = fabs(Welch(tracesFixe, tracesAleatoire, N_TRACES)) > THRESHOLD;
    printf("  d=1 | t = %7.3f | %s\n",
           Welch(tracesFixe, tracesAleatoire, N_TRACES),
           fuiteDetectee ? "[FUITE DETECTEE]" : "[OK] pas de fuite");

    free(tracesFixe);
    free(tracesAleatoire);
}

void TVLA_DOM(int d) {
    double *tracesFixe = malloc(N_TRACES * sizeof(double));
    double *tracesAleatoire = malloc(N_TRACES * sizeof(double));

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageA = MasquerBool(0xDEADBEEF, d);
        BoolSharing partageB = MasquerBool(0xCAFEBABE, d);
        BoolSharing partageResultat;
        DOM_AND(&partageA, &partageB, &partageResultat);
        tracesFixe[i] = Leakage(&partageResultat);
    }

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageA = MasquerBool(RandShare(), d);
        BoolSharing partageB = MasquerBool(RandShare(), d);
        BoolSharing partageResultat;
        DOM_AND(&partageA, &partageB, &partageResultat);
        tracesAleatoire[i] = Leakage(&partageResultat);
    }

    AfficherResultat(Welch(tracesFixe, tracesAleatoire, N_TRACES), d);

    free(tracesFixe);
    free(tracesAleatoire);
}

void TVLA_A2B(int d) {
    double *tracesFixe = malloc(N_TRACES * sizeof(double));
    double *tracesAleatoire = malloc(N_TRACES * sizeof(double));

    for (int i = 0; i < N_TRACES; i++) {
        ArithSharing partageArith = MasquerArith(0xDEADBEEF, d);
        BoolSharing partageBool;
        A2B(&partageArith, &partageBool);
        tracesFixe[i] = Leakage(&partageBool);
    }

    for (int i = 0; i < N_TRACES; i++) {
        ArithSharing partageArith = MasquerArith(RandShare(), d);
        BoolSharing partageBool;
        A2B(&partageArith, &partageBool);
        tracesAleatoire[i] = Leakage(&partageBool);
    }

    AfficherResultat(Welch(tracesFixe, tracesAleatoire, N_TRACES), d);

    free(tracesFixe);
    free(tracesAleatoire);
}

void TVLA_B2A_Classic(int d) {
    double *tracesFixe = malloc(N_TRACES * sizeof(double));
    double *tracesAleatoire = malloc(N_TRACES * sizeof(double));

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageBool = MasquerBool(0xDEADBEEF, d);
        ArithSharing partageArith;
        B2A_Classic(&partageBool, &partageArith);
        tracesFixe[i] = LeakageArith(&partageArith);
    }

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageBool = MasquerBool(RandShare(), d);
        ArithSharing partageArith;
        B2A_Classic(&partageBool, &partageArith);
        tracesAleatoire[i] = LeakageArith(&partageArith);
    }

    AfficherResultat(Welch(tracesFixe, tracesAleatoire, N_TRACES), d);

    free(tracesFixe);
    free(tracesAleatoire);
}

void TVLA_B2X2A(int d) {
    double *tracesFixe = malloc(N_TRACES * sizeof(double));
    double *tracesAleatoire = malloc(N_TRACES * sizeof(double));

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageBool = MasquerBool(0xDEADBEEF, d);
        ArithSharing partageArith;
        B2X2A(&partageBool, &partageArith);
        tracesFixe[i] = LeakageArith(&partageArith);
    }

    for (int i = 0; i < N_TRACES; i++) {
        BoolSharing partageBool = MasquerBool(RandShare(), d);
        ArithSharing partageArith;
        B2X2A(&partageBool, &partageArith);
        tracesAleatoire[i] = LeakageArith(&partageArith);
    }

    AfficherResultat(Welch(tracesFixe, tracesAleatoire, N_TRACES), d);

    free(tracesFixe);
    free(tracesAleatoire);
}

int main(void) {
    printf("___ Tests TVLA (%d traces, seuil = %.1f) ___\n\n",
           N_TRACES, THRESHOLD);
    RandInit(67676767);

    int ordres[] = {1, 2, 3, 4};
    int nombreOrdres = 4;

    printf("--- ISW_AND ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        TVLA_ISW(ordres[i]);

    printf("\n--- Trichina_AND ---\n");
    TVLA_Trichina();

    printf("\n--- DOM_AND ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        TVLA_DOM(ordres[i]);

    printf("\n--- A2B ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        TVLA_A2B(ordres[i]);

    printf("\n--- B2A_Classic ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        TVLA_B2A_Classic(ordres[i]);

    printf("\n--- B2X2A ---\n");
    for (int i = 0; i < nombreOrdres; i++)
        TVLA_B2X2A(ordres[i]);

    return 0;
}
