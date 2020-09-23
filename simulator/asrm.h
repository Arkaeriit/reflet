/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#ifndef ASRM
#define ASRM

#include "constants.h"
#include "opperand.h"
#include <stdlib.h>
#include <stdio.h>

struct asrm_config {
    word_t word_size;
    word_t word_size_byte;
    word_t word_mask;
    word_t ram_size;
};

typedef struct asrm_struct {
    word_t* reg;
    ram_word_t* ram;
    struct asrm_config* config;
} asrm;

#define WR(vm) vm->reg[0]
#define CR(vm) vm->reg[13]
#define PC(vm) vm->reg[14]
#define SP(vm) vm->reg[15]

asrm* asrm_init();
void asrm_initRAM(asrm* vm);
void asrm_free(asrm* vm);
void asrm_run(asrm* vm);

#endif

