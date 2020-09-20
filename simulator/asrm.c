/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#include "asrm.h"

asrm* asrm_init(){
    asrm* ret = malloc(sizeof(asrm));
    ret->reg = malloc(sizeof(word_t) * sizeof(NUMBER_OF_REGISTERS));
    ret->ram = malloc(sizeof(ram_word_t) * sizeof(RAM_SIZE));
    return ret;
}

void asrm_free(asrm* vm){
    free(vm->reg);
    free(vm->ram);
    free(vm);
}

