#include "gadgets_and.h"

void ISW_AND(BoolSharing *x, BoolSharing *y, BoolSharing *z) {
    int nombreShares = 2 * x->d + 1;
    Share u[MAX_SHARES][MAX_SHARES] = {0};

    for (int i = 0; i < nombreShares; i++) {
        for (int j = i + 1; j < nombreShares; j++) {
            Share r = RandShare();
            u[i][j] = r;
            u[j][i] = (r ^ (x->s[i] & y->s[j])) ^ (x->s[j] & y->s[i]);
        }
    }

    z->d = x->d;
    for (int i = 0; i < nombreShares; i++) {
        z->s[i] = x->s[i] & y->s[i];
        for (int j = 0; j < nombreShares; j++) {
            if (j != i) {
                z->s[i] ^= u[i][j];
            }
        }
    }
}

void Trichina_AND(BoolSharing *x, BoolSharing *y, BoolSharing *z) {
    Share random = RandShare();

    z->s[0] = ((random ^ (x->s[0] & y->s[0])) ^ (x->s[1] & y->s[0])) ^ (x->s[0] & y->s[1]);
    z->s[1] = random ^ (x->s[1] & y->s[1]);
    z->d = 1;
}

void DOM_AND(BoolSharing *x, BoolSharing *y, BoolSharing *z) {
    int nombreShares = x->d + 1;
    Share u[MAX_SHARES][MAX_SHARES] = {0};

    for (int i = 0; i < nombreShares; i++) {
        for (int j = i + 1; j < nombreShares; j++) {
            Share r = RandShare();
            u[i][j] = (x->s[i] & y->s[j]) ^ r;
            u[j][i] = (x->s[j] & y->s[i]) ^ r;
        }
    }

    z->d = x->d;
    for (int i = 0; i < nombreShares; i++) {
        z->s[i] = x->s[i] & y->s[i];
        for (int j = 0; j < nombreShares; j++) {
            if (j != i) {
                z->s[i] ^= u[i][j];
            }
        }
    }
}

void SecOR(BoolSharing *x, BoolSharing *y, BoolSharing *z) {
    BoolSharing nonX, nonY;
    SecNOT(x, &nonX);
    SecNOT(y, &nonY);
    DOM_AND(&nonX, &nonY, z);
    SecNOT(z, z);
}
