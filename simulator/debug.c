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
    sprintf(ret, "Instruction n°%" WORD_PX " , address: %" WORD_PX ", value :%x.",vm->debug->steps, PC(vm), instruction);
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
            if(instruction == SLP){
                sprintf(ret, "SLP: sleeping, value of the working register: %" WORD_PX "", WR(vm));
            }else if(instruction == JMP){
                sprintf(ret,"JMP: jumping to 0x%" WORD_PX "",WR(vm));
            }else if(instruction == JIF){
                sprintf(ret,"JIF: conditional jump to 0x%" WORD_PX ". Status register: 0x%" WORD_PX "",WR(vm), SR(vm));
            }else if(instruction == POP){
                sprintf(ret,"POP: poping stack");
            }else if(instruction == PUSH){
                sprintf(ret, "PUSH: pushing value to stack");
            }else if(instruction == CALL){
                sprintf(ret,"CALL: calling function at 0x%" WORD_PX "",WR(vm));
            }else if(instruction == RET){
                sprintf(ret, "RET: returning from last call");
            }else if(instruction == QUIT){
                sprintf(ret, "QUIT: stopping the simulation");
            }else if(instruction == DEBUG){
                sprintf(ret, "DEBUG: printing a debug message");
            }else if(instruction == CMPNOT){
                sprintf(ret, "CMPNOT: flipping the comparaison bit of the status register (%" WORD_PX ")", SR(vm));
            }else if(instruction == RETINT){
                sprintf(ret, "RETINT: returning from interruption routine");
            }else if(isSETINT(instruction)){
                sprintf(ret, "SETINT: setting the routine of interrupt %i to %" WORD_PX, instruction & 3, WR(vm));
            }else{
                sprintf(ret, "Unknown instruction");
            }
            break;
        case SET:
            sprintf(ret, "SET: setting WR to %x", reg);
            break;
        case READ:
            sprintf(ret, "READ: setting WR to value of register %i, which is %" WORD_PX "", reg, vm->reg[reg]);
            break;
        case CPY:
            sprintf(ret, "CPY: writing the value %" WORD_PX " to the register %i",WR(vm), reg);
            break;
        case ADD:
            sprintf(ret, "ADD: adding to the working register (%" WORD_PX ") the value of register %i (%" WORD_PX ")", WR(vm), reg, vm->reg[reg]);
            break;
        case SUB:
            sprintf(ret, "SUB: substracting to the working register (%" WORD_PX ") the value of register %i (%" WORD_PX ")", WR(vm), reg, vm->reg[reg]);
            break;
        case AND:
            sprintf(ret, "AND: logical and between the working register (%" WORD_PX ") and the register %i (%" WORD_PX ")", WR(vm), reg, vm->reg[reg]);
            break;
        case OR:
            sprintf(ret, "OR: logical or between the working register (%" WORD_PX ") and the register %i (%" WORD_PX ")", WR(vm), reg, vm->reg[reg]);
            break;
        case XOR:
            sprintf(ret, "XOR: logical xor between the working register (%" WORD_PX ") and the register %i (%" WORD_PX ")", WR(vm), reg, vm->reg[reg]);
            break;
        case NOT:
            sprintf(ret, "NOT: logical not of register %i (%" WORD_PX ")", reg, vm->reg[reg]);
            break;
        case LSL:
            sprintf(ret, "LSL: shifting working register (%" WORD_PX ") of %" WORD_PX " bits (from reg %i)", WR(vm), vm->reg[reg], reg);
            break;
        case LSR:
            sprintf(ret, "LSR: shifting working register (%" WORD_PX ") of %" WORD_PX " bits (from reg %i)", WR(vm), vm->reg[reg], reg);
            break;
        case EQ:
            sprintf(ret, "EQ: comparing working register (%" WORD_PX ") and register %i (%" WORD_PX ")", WR(vm), reg, vm->reg[reg]);
            break;
        case LES:
            sprintf(ret, "LES: comparing working register (%" WORD_PX ") and register %i (%" WORD_PX ")", WR(vm), reg, vm->reg[reg]);
            break;
        case STR:
            sprintf(ret, "STR: storing working register %" WORD_PX " at address in register %i (0x%" WORD_PX ")",WR(vm), reg, vm->reg[reg]);
            break;
        case LOAD:
            sprintf(ret, "LOAD: loading the content of address %" WORD_PX " (comming from register %i)", vm->reg[reg], reg);
            break;
        default:
            sprintf(ret, "Error in the reading of the instruction at address %" WORD_PX ".", PC(vm) - 1);
    }
    return ret;
}

