#ifndef ASSEMBLEXX
#define ASSEMBLEXX

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include <stdio.h>

void aXX_freeElems(uint8_t n,char** elems);
uint8_t aXX_preprocessLine(char* line,char** list);

#endif
