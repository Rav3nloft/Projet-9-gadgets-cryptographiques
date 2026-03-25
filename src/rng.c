#include "rng.h"

static uint32_t xorshiftState = 67676767;
static uint32_t randCounter = 0;

static uint32_t Xorshift32(void) {
    uint32_t x = xorshiftState;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    xorshiftState = x;
    return x;
}

void RandInit(uint32_t seed) {
    xorshiftState = seed;
    randCounter = 0;
}

Share RandShare(void) {
    randCounter++;
    return Xorshift32();
}

void RandFill(Share *s, int n) {
    for (int i = 0; i < n; i++) {
        s[i] = RandShare();
    }
}

void RandZeroSharing(Share *r, int d) {
    Share accumulateur = 0;
    for (int i = 0; i < d; i++) {
        r[i] = RandShare();
        accumulateur ^= r[i];
    }
    r[d] = accumulateur;
}

BoolSharing MasquerBool(Share secret, int d) {
    BoolSharing x;
    x.d = d;
    Share accumulateur = secret;
    for (int i = 0; i < d; i++) {
        x.s[i] = RandShare();
        accumulateur ^= x.s[i];
    }
    x.s[d] = accumulateur;
    return x;
}

BoolSharing MasquerISW(Share secret, int d) {
    int nombreShares = 2 * d + 1;
    BoolSharing x;
    x.d = d;
    Share accumulateur = secret;
    for (int i = 0; i < nombreShares - 1; i++) {
        x.s[i] = RandShare();
        accumulateur ^= x.s[i];
    }
    x.s[nombreShares - 1] = accumulateur;
    return x;
}SS

ArithSharing MasquerArith(Share secret, int d) {
    ArithSharing z;
    z.d = d;
    z.q = 0;
    Share accumulateur = secret;
    for (int i = 0; i < d; i++) {
        z.s[i] = RandShare();
        accumulateur -= z.s[i];
    }
    z.s[d] = accumulateur;
    return z;
}

Share Demasquer(BoolSharing *x) {
    Share accumulateur = 0;
    for (int i = 0; i <= x->d; i++)
        accumulateur ^= x->s[i];
    return accumulateur;
}

Share DemasquerISW(BoolSharing *x) {
    int nombreShares = 2 * x->d + 1;
    Share accumulateur = 0;
    for (int i = 0; i < nombreShares; i++)
        accumulateur ^= x->s[i];
    return accumulateur;
}

Share DemasquerArith(ArithSharing *z) {
    Share accumulateur = 0;
    for (int i = 0; i <= z->d; i++)
        accumulateur += z->s[i];
    return accumulateur;
}

void RandResetCounter(void) {
    randCounter = 0;
}

uint32_t RandGetCounter(void) {
    return randCounter;
}
