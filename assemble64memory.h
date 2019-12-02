#ifndef ASSEMBLE64MEMORY
#define ASSEMBLE64MEMORY

#include <inttypes.h>
#include "assembleXX.h"

uint8_t a64m_free(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_ask(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_ask_byte(char** elems, uint8_t n, uint64_t* fullCode);

#endif

