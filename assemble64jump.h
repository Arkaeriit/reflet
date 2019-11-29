#ifndef ASSEMBLE64JUMP
#define ASSEMBLE64JUMP

#include "assemble64.h"

uint8_t a64j_cmp(char** elems,uint8_t n,uint64_t* fullCode);
uint8_t a64j_lab(char** elems,uint8_t n,char* ret);
uint8_t a64j_jmp(char** elems,uint8_t n,char* ret);
uint8_t a64j_jz(char** elems,uint8_t n,char* ret);
uint8_t a64j_jb(char** elems,uint8_t n,char* ret);
uint8_t a64j_jse(char** elems,uint8_t n,char* ret);

uint8_t a64j_call(char** elems,uint8_t n,char* ret);
uint8_t a64j_ret(char** elems,uint8_t n,uint64_t* fullCode);

#endif

