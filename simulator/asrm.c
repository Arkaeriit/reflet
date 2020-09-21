/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#include "asrm.h"
static void run_inst(asrm* vm);

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

/*
 * Run the program in the vm's RAM
 */
void asrm_run(asrm* vm){
    while(PC(vm) < RAM_SIZE)
        run_inst(vm);
}

/*
 * Run a single instruction from the RAM
 */
static void run_inst(asrm* vm){
    uint8_t instruction = (uint8_t) vm->ram[PC(vm)];
    uint8_t opperand = (instruction & 0xF0) >> 4;
    uint8_t reg = instruction & 0x0F;
    PC(vm)++;
    switch(opperand){
        case 0:
            if(isSlp(instruction)){
                break;
            }else if(instruction == JMP){
                LR(vm) = PC(vm);
                PC(vm) = WR(vm);
            }else if(instruction == JIF){
                if(CR(vm)){
                    LR(vm) = PC(vm);
                    PC(vm) = WR(vm);
                }
            }else if(instruction == RET){
                PC(vm) = LR(vm) + 1;
            }else{
                fprintf(stderr, "Warning, unknow instruction (%X) at address %X.\n",instruction, PC(vm) - 1);
            }
            break;
        case SET:
            WR(vm) = reg;
            break;
        case READ:
            WR(vm) = vm->reg[reg];
            break;
        case CPY:
            vm->reg[reg] = WR(vm);
            break;
        case ADD:
            WR(vm) += vm->reg[reg];
            break;
        case SUB:
            WR(vm) -= vm->reg[reg];
            break;
        case AND:
            WR(vm) &= vm->reg[reg];
            break;
        case OR:
            WR(vm) |= vm->reg[reg];
            break;
        case XOR:
            WR(vm) ^= vm->reg[reg];
            break;
        case NOT:
            WR(vm) = ~(vm->reg[reg]);
            break;
        case LSL:
            WR(vm) = WR(vm) << vm->reg[reg];
            break;
        case LSR:
            WR(vm) = WR(vm) >> vm->reg[reg];
            break;
        case EQ:
            CR(vm) = ((WR(vm) == vm->reg[reg]) ? ~0 : 0);
            break;
        case LES:
            CR(vm) = ((WR(vm) < vm->reg[reg]) ? ~0 : 0);
            break;
        case STR:
            vm->ram[vm->reg[reg]] = WR(vm);
            break;
        case LOAD:
            WR(vm) = vm->ram[vm->reg[reg]];
            break;
        default:
            fprintf(stderr, "Error in the reading of the instruction at address %X.\n", PC(vm) - 1);
    }
}

