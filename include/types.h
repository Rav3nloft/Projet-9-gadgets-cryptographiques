#ifndef __TYPES__H__
#define __TYPES__H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ORDER  4
#define MAX_SHARES (2 * MAX_ORDER + 1)	//ISW nécessite 2d+1 (d'où pas d+1) pas embêtant pour le projet mais devrai être rendu spécifique
#define WORD_SIZE  32

typedef uint32_t Share;

typedef struct {
    Share s[MAX_SHARES];
    int d; 				//ordre de protection (nb de cable à dépasser pour casser le masque)
} BoolSharing;

typedef struct {
    Share s[MAX_SHARES];
    int d;		
    uint32_t q; 			//module
} ArithSharing;

#endif
