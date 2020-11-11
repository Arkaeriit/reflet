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
#include <stdbool.h>

struct asrm_config_int {
    bool enable;
    int freq;
};

struct asrm_int {
    word_t routine;
    int count_up;
};

struct asrm_int_level {
    int level;
    int stack_depth;
    int level_stack[5];
    word_t routine_stack[5];
};

struct asrm_config {
    word_t word_size;
    word_t word_size_byte;
    word_t word_mask;
    word_t ram_size;
    word_t tx_cmd;
    word_t tx_data;
    word_t rx_cmd;
    word_t rx_data;
    struct asrm_config_int* ints[4];
};

struct asrm_debug {
    bool enable;
    FILE* file;
    word_t steps;
};

typedef struct asrm_struct {
    word_t* reg;
    ram_word_t* ram;
    struct asrm_config* config;
    struct asrm_debug* debug;
    struct asrm_int* ints[4];
    struct asrm_int_level* int_level;
    bool active;
} asrm;

#include "debug.h"

#define WR(vm) vm->reg[0]
#define SR(vm) vm->reg[13]
#define PC(vm) vm->reg[14]
#define SP(vm) vm->reg[15]

asrm* asrm_init();
void asrm_initRAM(asrm* vm);
void asrm_free(asrm* vm);
void asrm_run(asrm* vm);

#endif

