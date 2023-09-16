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
    bool extended_io;
};

struct reflet_debug {
    bool enable;
    FILE* file;
    word_t steps;
};

typedef struct reflet_struct {
	union {
		word_t reg[16];
		struct {
			word_t WR;
			word_t R1;
			word_t R2;
			word_t R3;
			word_t R4;
			word_t R5;
			word_t R6;
			word_t R7;
			word_t R8;
			word_t R9;
			word_t R10;
			word_t R11;
			word_t R12;
			word_t SR;
			word_t PC;
			word_t SP;
		};
	};
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

#define SR_COMP_MASK 0x01
#define SR_INT0_MASK 0x02
#define SR_INT1_MASK 0x04
#define SR_INT2_MASK 0x08
#define SR_INT3_MASK 0x10
#define SR_TRAP_MASK 0x20

#define SR_REDUCED_POS 8

reflet* reflet_init();
void reflet_initRAM(reflet* vm);
void reflet_free(reflet* vm);
void reflet_run(reflet* vm);

#endif

