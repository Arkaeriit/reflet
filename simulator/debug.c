/*---------------------------------------------------\
|This file contain functions used to writ debug logs |
|to get info about the state of the processor at each|
|steps of the simulation.                            |
\---------------------------------------------------*/

#include "debug.h"

//static functions
static char* dataIntro(reflet* vm);
static char* dataAboutInstruction(reflet* vm);

/*
 * try to log the current state of the processor in the log file
 */
void debugLog(reflet* vm){
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
static char* dataIntro(reflet* vm){
    char* ret = malloc(1024);
    uint8_t instruction = (uint8_t) vm->ram[vm->PC];
    sprintf(ret, "Instruction n°%" WORD_PX " , address: %" WORD_PX ", value :%x.",vm->debug->steps, vm->PC, instruction);
    return ret;
}

/*
 * return a description of the effetcs of the current instruction
 */
static char* dataAboutInstruction(reflet* vm){
    char* ret = malloc(1024);
    uint8_t instruction = (uint8_t) vm->ram[vm->PC];
    uint8_t opperand = (instruction & 0xF0) >> 4;
    uint8_t reg = instruction & 0x0F;
    switch(opperand){
        case 0:
            switch(instruction) {
                case SLP:
                    sprintf(ret, "SLP: sleeping, value of the working register: %" WORD_PX "", vm->WR);
                    break;
                case CC2:
                    sprintf(ret,"CC2: Computing the two's complement of 0x%" WORD_PX "",vm->WR);
                    break;
                case JIF:
                    sprintf(ret,"JIF: conditional jump to 0x%" WORD_PX ". Status register: 0x%" WORD_PX "",vm->WR, vm->SR);
                    break;
                case POP:
                    sprintf(ret,"POP: poping stack");
                    break;
                case PUSH:
                    sprintf(ret, "PUSH: pushing value to stack");
                    break;
                case CALL:
                    sprintf(ret,"CALL: calling function at 0x%" WORD_PX "",vm->WR);
                    break;
                case RET:
                    sprintf(ret, "RET: returning from last call");
                    break;
                case QUIT:
                    sprintf(ret, "QUIT: stopping the simulation");
                    break;
                case DEBUG:
                    sprintf(ret, "DEBUG: printing a debug message");
                    break;
                case CMPNOT:
                    sprintf(ret, "CMPNOT: flipping the comparaison bit of the status register (%" WORD_PX ")", vm->SR);
                    break;
                case RETINT:
                    sprintf(ret, "RETINT: returning from interruption routine");
                    break;
                default:
                    if(isSETINT(instruction)){
                        sprintf(ret, "SETINT: setting the routine of interrupt %i to %" WORD_PX, instruction & 3, vm->WR);
                    }else{
                        sprintf(ret, "Unknown instruction");
                    }
            }
            break;
        case SET:
            sprintf(ret, "SET: setting WR to %x", reg);
            break;
        case READ:
            sprintf(ret, "READ: setting WR to value of register %i, which is %" WORD_PX "", reg, vm->reg[reg]);
            break;
        case CPY:
            sprintf(ret, "CPY: writing the value %" WORD_PX " to the register %i",vm->WR, reg);
            break;
        case ADD:
            sprintf(ret, "ADD: adding to the working register (%" WORD_PX ") the value of register %i (%" WORD_PX ")", vm->WR, reg, vm->reg[reg]);
            break;
        case SUB:
            sprintf(ret, "SUB: substracting to the working register (%" WORD_PX ") the value of register %i (%" WORD_PX ")", vm->WR, reg, vm->reg[reg]);
            break;
        case AND:
            sprintf(ret, "AND: logical and between the working register (%" WORD_PX ") and the register %i (%" WORD_PX ")", vm->WR, reg, vm->reg[reg]);
            break;
        case OR:
            sprintf(ret, "OR: logical or between the working register (%" WORD_PX ") and the register %i (%" WORD_PX ")", vm->WR, reg, vm->reg[reg]);
            break;
        case XOR:
            sprintf(ret, "XOR: logical xor between the working register (%" WORD_PX ") and the register %i (%" WORD_PX ")", vm->WR, reg, vm->reg[reg]);
            break;
        case NOT:
            sprintf(ret, "NOT: logical not of register %i (%" WORD_PX ")", reg, vm->reg[reg]);
            break;
        case LSL:
            sprintf(ret, "LSL: shifting working register (%" WORD_PX ") of %" WORD_PX " bits (from reg %i)", vm->WR, vm->reg[reg], reg);
            break;
        case LSR:
            sprintf(ret, "LSR: shifting working register (%" WORD_PX ") of %" WORD_PX " bits (from reg %i)", vm->WR, vm->reg[reg], reg);
            break;
        case EQ:
            sprintf(ret, "EQ: comparing working register (%" WORD_PX ") and register %i (%" WORD_PX ")", vm->WR, reg, vm->reg[reg]);
            break;
        case LES:
            sprintf(ret, "LES: comparing working register (%" WORD_PX ") and register %i (%" WORD_PX ")", vm->WR, reg, vm->reg[reg]);
            break;
        case STR:
            sprintf(ret, "STR: storing working register %" WORD_PX " at address in register %i (0x%" WORD_PX ")",vm->WR, reg, vm->reg[reg]);
            break;
        case LOAD:
            sprintf(ret, "LOAD: loading the content of address %" WORD_PX " (comming from register %i)", vm->reg[reg], reg);
            break;
        default:
            sprintf(ret, "Error in the reading of the instruction at address %" WORD_PX ".", vm->PC - 1);
    }
    return ret;
}

