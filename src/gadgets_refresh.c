#include "gadgets_refresh.h"

void SecREF(BoolSharing *x, BoolSharing *y) {
    Share r[MAX_SHARES];
    RandZeroSharing(r, x->d);
    y->d = x->d;
    for (int i = 0; i <= x->d; i++) {
        y->s[i] = x->s[i] ^ r[i];
    }
}

void SecEXP(BoolSharing *x, BoolSharing *y) {
    int nombreShares = x->d + 1;
    Share r[MAX_SHARES];
    RandFill(r, nombreShares);
    y->d = 2 * x->d + 1;
    for (int i = 0; i < nombreShares; i++) {
        y->s[i] = x->s[i] ^ r[i];
        y->s[nombreShares + i] = r[i];
    }
}

void RefreshMasks(BoolSharing *x, BoolSharing *y) {
    BoolSharing tmp;
    tmp = *x;
    for (int j = 0; j < x->d; j++) {
        Share r[MAX_SHARES];
        RandZeroSharing(r, x->d - j);
        for (int i = j; i <= x->d; i++) {
            tmp.s[i] ^= r[i - j];
        }
    }
    *y = tmp;
}

void FullXOR(BoolSharing *x, Share *out) {
    BoolSharing refreshed;
    RefreshMasks(x, &refreshed);
    *out = 0;
    for (int i = 0; i <= refreshed.d; i++) {
        *out ^= refreshed.s[i];
    }
}
