/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#include "asrm.h"

/*
 * Create an asrm struct with the content described in content.h
 *  return:
 *      A pointer to the struct
 */
asrm* asrm_init(){
    asrm* ret = malloc(sizeof(asrm));
    ret->reg = calloc(NUMBER_OF_REGISTERS, sizeof(word_t));
    ret->ram = calloc(RAM_SIZE, sizeof(ram_word_t));
    //Input reset values not equal to 0
    CR(ret) = ~0;
    PC(ret) = START_CHAR;
    return ret;
}

/*
 * Free an asrm struct
 */
void asrm_free(asrm* vm){
    free(vm->reg);
    free(vm->ram);
    free(vm);
}

