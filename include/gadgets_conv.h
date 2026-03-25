#ifndef __GADGETS__CONV__H__
#define __GADGETS__CONV__H__

#include "gadgets_and.h"

// Addition sécurisée de deux partages Booléens — t-NI
// Calcule le partage Booléen de (XOR(x^i) + XOR(y^i)) mod 2^32
void SecADD(BoolSharing *x, BoolSharing *y, BoolSharing *s);

// Conversion Arithmétique → Booléen — t-NI
void A2B(ArithSharing *z, BoolSharing *B);

// Conversion Booléen → Arithmétique (classique) — t-NI
// Utilise A2B + SecADD
void B2A_Classic(BoolSharing *B, ArithSharing *A);

// Conversion Booléen → Arithmétique (optimisée) — t-NI
void B2X2A(BoolSharing *B, ArithSharing *A);

#endif
