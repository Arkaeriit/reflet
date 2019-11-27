#ifndef EXEC64
#define EXEC64

#include "ReadBin.h"
#include "constants.h"

#define REG1 0xFC00 /*Mask to get the 1st register*/
#define REG2 0x3F0000

#define NUM1 0xFFFFFFFFFFFF0000/*Mask to get the numeric value if there is only 1 register*/

int e64_execute(vm_64* vm);

uint64_t e64_num1(uint64_t inst);
uint8_t e64_reg1(uint64_t inst);
uint8_t e64_reg2(uint64_t inst);
#endif

