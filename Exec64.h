#ifndef EXEC64
#define EXEC64

#include "ReadBin.h"
#include "constants.h"

int e64_execute(vm_64* vm);

uint64_t e64_num0(uint64_t inst);
uint64_t e64_num1(uint64_t inst);
uint8_t e64_reg1(uint64_t inst);
uint8_t e64_reg2(uint64_t inst);
uint8_t e64_reg3(uint64_t inst);

void e64_cmp(bool* flags, uint64_t num1, uint64_t num2);
#endif

