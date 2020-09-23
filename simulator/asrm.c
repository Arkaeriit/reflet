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
    ret->ram = NULL; //Left blank, should be written a value depending of its configuration
    //default config
    struct asrm_config* conf = malloc(sizeof(struct asrm_config));
    conf->word_size = WORD_SIZE;
    conf->word_size_byte = WORD_SIZE_BYTE;
    conf->word_mask = WORD_MASK;
    conf->ram_size = RAM_SIZE;
    ret->config = conf;
    //Input reset values not equal to 0
    CR(ret) = ~0;
    PC(ret) = START_CHAR;
    return ret;
}

/*
 * Setup the ram of an ASRM struct depending of its configuration
 */
void asrm_initRAM(asrm* vm){
    vm->ram = calloc(vm->config->ram_size, sizeof(ram_word_t));
}

/*
 * Free an asrm struct
 */
void asrm_free(asrm* vm){
    free(vm->config);
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
    word_t reg_mask = vm->config->word_mask;
    uint8_t instruction = (uint8_t) vm->ram[PC(vm)];
    uint8_t opperand = (instruction & 0xF0) >> 4;
    uint8_t reg = instruction & 0x0F;
    PC(vm)++;
    switch(opperand){
        case 0:
            if(isSlp(instruction)){
                break;
            }else if(instruction == JMP){
                PC(vm) = WR(vm);
            }else if(instruction == JIF){
                if(CR(vm))
                    PC(vm) = WR(vm);
            }else if(instruction == POP){
                SP(vm)--;
                WR(vm) = (reg_mask) & vm->ram[SP(vm)];
            }else if(instruction == PUSH){
                vm->ram[SP(vm)] = WR(vm);
                SP(vm)++;
            }else if(instruction == CALL){
                vm->ram[SP(vm)-1] = PC(vm); //-1 because we already updated the PC and we want to store the address of the call
                SP(vm)++;
            }else if(instruction == RET){
                SP(vm)--;
                PC(vm) = vm->ram[SP(vm)];
            }else{
                fprintf(stderr, "Warning, unknow instruction (%X) at address %" WORD_P ".\n",instruction, PC(vm) - 1);
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
            fprintf(stderr, "Error in the reading of the instruction at address %" WORD_P ".\n", PC(vm) - 1);
    }
}

