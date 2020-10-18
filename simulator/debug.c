/*---------------------------------------------------\
|This file contain functions used to writ debug logs |
|to get info about the state of the processor at each|
|steps of the simulation.                            |
\---------------------------------------------------*/

#include "debug.h"

//static functions
static char* dataIntro(asrm* vm);
static char* dataAboutInstruction(asrm* vm);

/*
 * try to log the current state of the processor in the log file
 */
void debugLog(asrm* vm){
    if(!vm->debug->enable)
        return;
    char* intro = dataIntro(vm);
    char* data = dataAboutInstruction(vm);
    fprintf(vm->debug->file, "%s %s\n",intro, data);
    free(intro);
    free(data);
    vm->debug->steps++;
}

/*
 * return a quick overview of the state of the processor
 */
static char* dataIntro(asrm* vm){
    char* ret = malloc(1024);
    uint8_t instruction = (uint8_t) vm->ram[PC(vm)];
    sprintf(ret, "Instruction n°%lu , address: %lx, value :%x.",vm->debug->steps, PC(vm), instruction);
    return ret;
}

/*
 * return a description of the effetcs of the current instruction
 */
static char* dataAboutInstruction(asrm* vm){
    char* ret = malloc(1024);
    uint8_t instruction = (uint8_t) vm->ram[PC(vm)];
    uint8_t opperand = (instruction & 0xF0) >> 4;
    uint8_t reg = instruction & 0x0F;
    switch(opperand){
        case 0:
            if(isSlp(instruction)){
                sprintf(ret, "SLP: sleeping, value of the working register: %lu", WR(vm));
            }else if(instruction == JMP){
                sprintf(ret,"JMP: jumping to 0x%lx",WR(vm));
            }else if(instruction == JIF){
                sprintf(ret,"JIF: conditional jump toi 0x%lx. Status register: 0x%lx",WR(vm), SR(vm));
            }else if(instruction == POP){
                sprintf(ret,"POP: poping stack");
            }else if(instruction == PUSH){
                sprintf(ret, "PUSH: pushing value to stack");
            }else if(instruction == CALL){
                sprintf(ret,"CALL: calling function at 0x%lx",WR(vm));
            }else if(instruction == RET){
                sprintf(ret, "RET: returning from last call");
            }else if(instruction == QUIT){
                sprintf(ret, "QUIT: stopping the simulation");
            }else{
                sprintf(ret, "Unknown instruction");
            }
            break;
        case SET:
            sprintf(ret, "SET: setting WR to %i", reg);
            break;
        case READ:
            sprintf(ret, "READ: setting WR to valie of register %i, which is %lu", reg, vm->reg[reg]);
            break;
        case CPY:
            sprintf(ret, "CPY: writing the value %lu to the register %i",WR(vm), reg);
            break;
        case ADD:
            sprintf(ret, "ADD: adding to the working register (%lu) the value of register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case SUB:
            sprintf(ret, "SUB: substracting to the working register (%lu) the value of register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case AND:
            sprintf(ret, "AND: logical and between the working register (%lu) and the register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case OR:
            sprintf(ret, "OR: logical or between the working register (%lu) and the register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case XOR:
            sprintf(ret, "XOR: logical xor between the working register (%lu) and the register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case NOT:
            sprintf(ret, "NOT: logical not between the working register (%lu) and the register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case LSL:
            sprintf(ret, "LSL: shifting working register (%lu) of %lu bits (from reg %i)", WR(vm), vm->reg[reg], reg);
            break;
        case LSR:
            sprintf(ret, "LSR: shifting working register (%lu) of %lu bits (from reg %i)", WR(vm), vm->reg[reg], reg);
            break;
        case EQ:
            sprintf(ret, "EQ: comparing working register (%lu) and register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case LES:
            sprintf(ret, "LES: comparing working register (%lu) and register %i (%lu)", WR(vm), reg, vm->reg[reg]);
            break;
        case STR:
            sprintf(ret, "STR: storing register %lu at address in register %i (0x%lx)",WR(vm), vm->reg[reg], reg);
            break;
        case LOAD:
            sprintf(ret, "LOAD: puting the content of address %lx in register %i", WR(vm), reg);
            break;
        default:
            sprintf(ret, "Error in the reading of the instruction at address %" WORD_P ".", PC(vm) - 1);
    }
    return ret;
}

