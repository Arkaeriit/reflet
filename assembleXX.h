#ifndef ASSEMBLEXX
#define ASSEMBLEXX

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "constants.h"
#include <stdio.h>

void aXX_freeElems(uint8_t n,char** elems);
uint8_t aXX_preprocessLine(char* line,char** list);
uint64_t aXX_readHex(char* number);
uint64_t aXX_readDec(char* number);

#endif
