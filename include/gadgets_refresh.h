#ifndef __GADGETS__REFRESH__H__
#define __GADGETS__REFRESH__H__

#include "gadgets_basic.h"

// Refresh fort d'un sharing Booléen — t-SNI
// Produit un nouveau sharing de la même valeur
void SecREF(BoolSharing *x, BoolSharing *y);

// Expansion d'un sharing Booléen — t-NI
// Double le nombre de shares :
// d+1 shares en entrée → 2*(d+1) shares en sortie
void SecEXP(BoolSharing *x, BoolSharing *y);

// Refresh multiple — t-SNI
// Applique d+1 SecREF successifs
void RefreshMasks(BoolSharing *x, BoolSharing *y);

// Démasquage sécurisé — t-NI
// Reconstruit la valeur secrète depuis ses shares
void FullXOR(BoolSharing *x, Share *out);

#endif
