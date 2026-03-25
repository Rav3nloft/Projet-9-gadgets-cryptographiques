#ifndef __GADGETS__BASIC__H__
#define __GADGETS__BASIC__H__

#include "rng.h"

// XOR Booléen masqué — t-NI
void SecXOR(BoolSharing *x, BoolSharing *y, BoolSharing *z);

// Négation Booléenne masquée — t-NI
void SecNOT(BoolSharing *x, BoolSharing *y);

#endif
