/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#ifndef ASRM
#define ASRM

#include "constants.h"
#include <stdlib.h>

typedef struct asrm_struct {
    word_t* reg;
    ram_word_t* ram;
} asrm;

asrm* asrm_init();
void asrm_free(asrm* vm);


#endif

