#ifndef __RNG__H__
#define __RNG__H__

#include "types.h"

// Initialise le générateur avec une graine
void RandInit(uint32_t seed);

// Génère un Share aléatoire
Share RandShare(void);

// Remplit un tableau de n Shares aléatoires
void RandFill(Share *buf, int n);

// Génère d+1 shares tels que :
// r[0] XOR r[1] XOR ... XOR r[d] = 0
// Requis par SecREF
void RandZeroSharing(Share *r, int d);

// Masque un secret en BoolSharing, ArithSharing ou BoolSharing ISW
BoolSharing MasquerBool(Share secret, int d);
BoolSharing MasquerISW(Share secret, int d);
ArithSharing MasquerArith(Share secret, int d);

// Démasque un BoolSharing ou un BoolSharing ISW
Share Demasquer(BoolSharing *x);
Share DemasquerISW(BoolSharing *x);

// Démasque un ArithSharing
Share DemasquerArith(ArithSharing *z);

// Compteur de randoms consommés
void RandResetCounter(void);
uint32_t RandGetCounter(void);

#endif
