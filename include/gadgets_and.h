#ifndef __GADGETS__AND__H__
#define __GADGETS__AND__H__

#include "gadgets_refresh.h"

void ISW_AND(BoolSharing *x, BoolSharing *y, BoolSharing *z);
void Trichina_AND(BoolSharing *x, BoolSharing *y, BoolSharing *z);
void DOM_AND(BoolSharing *x, BoolSharing *y, BoolSharing *z);

// OR masqué — utilise DOM_AND en interne
void SecOR(BoolSharing *x, BoolSharing *y, BoolSharing *z);

#endif
