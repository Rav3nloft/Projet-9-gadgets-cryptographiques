#include "gadgets_basic.h"

void SecXOR(BoolSharing *x, BoolSharing *y, BoolSharing *z) {
    z->d = x->d;
    for (int i = 0; i <= x->d; i++) {
        z->s[i] = x->s[i] ^ y->s[i];
    }
}

void SecNOT(BoolSharing *x, BoolSharing *y) {
    y->d = x->d;
    y->s[0] = ~x->s[0];
    for (int i = 1; i <= x->d; i++) {
        y->s[i] = x->s[i];
    }
}
