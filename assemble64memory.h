#ifndef ASSEMBLE64MEMORY
#define ASSEMBLE64MEMORY

#include <inttypes.h>
#include "assembleXX.h"
#include "assemble64.h"

uint8_t a64m_free(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_ask(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_ask_byte(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_str(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_ldr(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_str_byte(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_ldr_byte(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64m_data(char** elems, uint8_t n, char* ret);
uint8_t a64m_concatStrings(char** elems, uint8_t n, bool* isString);

#endif

