#ifndef ASSEMBLE64MATH
#define ASSEMBLE64MATH

#include <inttypes.h>
#include "constants.h"
#include "assemble64.h"

uint8_t a64m_add(char** elems,uint8_t n,uint64_t* fullCode);
uint8_t a64m_sub(char** elems,uint8_t n,uint64_t* fullCode);
uint8_t a64m_tim(char** elems,uint8_t n,uint64_t* fullCode);
uint8_t a64m_div(char** elems,uint8_t n,uint64_t* fullCode);
uint8_t a64m_mod(char** elems,uint8_t n,uint64_t* fullCode);

#endif

