#include "gadgets_conv.h"

BoolSharing MakeBoolean(Share valeur, int d) {
    BoolSharing partage;
    partage.d = d;
    Share accumulateur = valeur;
    for (int i = 0; i < d; i++) {
        partage.s[i] = RandShare();
        accumulateur ^= partage.s[i];
    }
    partage.s[d] = accumulateur;
    return partage;
}

void SecADD(BoolSharing *x, BoolSharing *y, BoolSharing *s) {
    int d = x->d;

    BoolSharing resultat;
    resultat.d = d;
    for (int i = 0; i <= d; i++)
        resultat.s[i] = 0;

    BoolSharing retenue;
    retenue.d = d;
    for (int i = 0; i <= d; i++)
        retenue.s[i] = 0;

    for (int bit = 0; bit < WORD_SIZE; bit++) {
        BoolSharing xb, yb;
        xb.d = yb.d = d;
        for (int i = 0; i <= d; i++) {
            xb.s[i] = (x->s[i] >> bit) & 1;
            yb.s[i] = (y->s[i] >> bit) & 1;
        }

        BoolSharing propagation;
        SecXOR(&xb, &yb, &propagation);

        BoolSharing bitSomme;
        SecXOR(&propagation, &retenue, &bitSomme);

        for (int i = 0; i <= d; i++)
            resultat.s[i] |= (bitSomme.s[i] & 1) << bit;

        BoolSharing generation;
        DOM_AND(&xb, &yb, &generation);

        BoolSharing propagationRetenue;
        DOM_AND(&propagation, &retenue, &propagationRetenue);

        BoolSharing nouvelleRetenue;
        SecOR(&generation, &propagationRetenue, &nouvelleRetenue);

        SecREF(&nouvelleRetenue, &retenue);
    }

    *s = resultat;
}

void A2B(ArithSharing *z, BoolSharing *B) {
    *B = MakeBoolean(z->s[0], z->d);
    for (int i = 1; i <= z->d; i++) {
        BoolSharing partageShare = MakeBoolean(z->s[i], z->d);
        BoolSharing tmp;
        SecADD(B, &partageShare, &tmp);
        *B = tmp;
    }
}

void B2A_Classic(BoolSharing *B, ArithSharing *A) {
    int d = B->d;
    A->d = d;
    A->q = 0;

    for (int i = 0; i < d; i++)
        A->s[i] = RandShare();

    ArithSharing partageNegatifR;
    partageNegatifR.d = d;
    partageNegatifR.q = 0;
    for (int i = 0; i < d; i++)
        partageNegatifR.s[i] = -(int32_t)A->s[i];
    partageNegatifR.s[d] = 0;

    BoolSharing partageNegatifRBool;
    A2B(&partageNegatifR, &partageNegatifRBool);

    BoolSharing resultatBool;
    SecADD(B, &partageNegatifRBool, &resultatBool);

    FullXOR(&resultatBool, &A->s[d]);
}

void B2X2A(BoolSharing *B, ArithSharing *A) {
    int d = B->d;
    A->d = d;
    A->q = 0;

    for (int i = 0; i < d; i++)
        A->s[i] = RandShare();

    BoolSharing resultat;
    SecNOT(B, &resultat);

    for (int i = 0; i < d; i++) {
        BoolSharing partageRi = MakeBoolean(A->s[i], d);
        BoolSharing tmp;
        SecADD(&resultat, &partageRi, &tmp);
        resultat = tmp;
    }

    BoolSharing resultatFinal;
    SecNOT(&resultat, &resultatFinal);

    FullXOR(&resultatFinal, &A->s[d]);
}
