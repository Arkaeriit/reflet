#ifndef ASSEMBLE64
#define ASSEMBLE64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "assembleXX.h"
#include "opperand.h"

//mask for the data
#define NUM1_MASK 0xFFFFFFFFFFFF
#define REG_MASK 0x3F

void a64_assemble(FILE* fin,FILE* fout);
uint8_t a64_compileLine(char** elems,uint8_t n,char* ret);

#endif

