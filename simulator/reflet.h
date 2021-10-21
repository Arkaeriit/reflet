/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#ifndef REFLET
#define REFLET

#include "constants.h"
#include "opperand.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct reflet_config_int {
    bool enable;
    int freq;
};

struct reflet_int {
    word_t routine;
    int count_up;
};

struct reflet_int_level {
    int level;
    int stack_depth;
    int level_stack[5];
    word_t routine_stack[5];
};

struct reflet_config {
    word_t word_size;
    word_t word_size_byte;
    word_t word_mask;
    word_t ram_size;
    word_t tx_cmd;
    word_t tx_data;
    word_t rx_cmd;
    word_t rx_data;
    struct reflet_config_int* ints[4];
};

struct reflet_debug {
    bool enable;
    FILE* file;
    word_t steps;
};

typedef struct reflet_struct {
    word_t* reg;
    ram_word_t* ram;
    struct reflet_config* config;
    struct reflet_debug* debug;
    struct reflet_int* ints[4];
    struct reflet_int_level* int_level;
    bool active;
    bool byte_mode;
} reflet;

#include "debug.h"

#define WR_REG 00
#define SR_REG 13
#define PC_REG 14
#define SP_REG 15

#define WR(vm) vm->reg[WR_REG]
#define SR(vm) vm->reg[SR_REG]
#define PC(vm) vm->reg[PC_REG]
#define SP(vm) vm->reg[SP_REG]

reflet* reflet_init();
void reflet_initRAM(reflet* vm);
void reflet_free(reflet* vm);
void reflet_run(reflet* vm);

#endif

