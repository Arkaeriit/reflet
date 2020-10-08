/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#include "asrm.h"
static void run_inst(asrm* vm);
static int byteExchanged(const asrm* vm){
static word_t loadWordRAM(const asrm* vm, word_t addr);
static void putRAMWord(asrm* vm, word_t addr, word_t content);
static void io(asrm* vm);
#define min(x, y) (x < y ? x : y)

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
    SR(ret) = 1;
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
    while(PC(vm) < vm->config->ram_size){
        run_inst(vm);
        io(vm);
    }
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
                if(SR(vm) & 1)
                    PC(vm) = WR(vm);
            }else if(instruction == POP){
                SP(vm) = (SP(vm) - 1) & reg_mask;
                WR(vm) = loadWordRAM(vm, SP(vm));
            }else if(instruction == PUSH){
                putRAMWord(vm, SP(vm), WR(vm));
                SP(vm) = (SP(vm) + 1) & reg_mask;
            }else if(instruction == CALL){
                putRAMWord(vm, SP(vm)-1, PC(vm)); //-1 because we already updated the PC and we want to store the address of the call
                SP(vm) = (SP(vm) + 1) & reg_mask;
            }else if(instruction == RET){
                SP(vm) = (SP(vm) - 1) & reg_mask;
                PC(vm) = loadWordRAM(vm, SP(vm));
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
            WR(vm) &= reg_mask;
            break;
        case SUB:
            WR(vm) -= vm->reg[reg];
            WR(vm) &= reg_mask;
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
            //TODO: fill the unused bits with the same value as the MSB
            WR(vm) = WR(vm) << vm->reg[reg];
            WR(vm) &= reg_mask;
            break;
        case LSR:
            WR(vm) = WR(vm) >> vm->reg[reg];
            WR(vm) &= reg_mask;
            break;
        case EQ:
            if(WR(vm) == vm->reg[reg]){
                SR(vm) |= 1;
            }else{
                SR(vm) = SR(vm) >> 1;
                SR(vm) = SR(vm) << 1;
            }
            break;
        case LES:
            if(WR(vm) < vm->reg[reg]){
                SR(vm) |= 1;
            }else{
                SR(vm) = SR(vm) >> 1;
                SR(vm) = SR(vm) << 1;
            }
            break;
        case STR:
            putRAMWord(vm, vm->reg[reg], WR(vm));
            break;
        case LOAD:
            WR(vm) = loadWordRAM(vm, vm->reg[reg]);
            break;
        default:
            fprintf(stderr, "Error in the reading of the instruction at address %" WORD_P ".\n", PC(vm) - 1);
    }
}

/*
 * Compute how many byte of data the processor should
 * exchange with the RAM
 *  Argument :
 *      vm : the asrm struct
 *  return :
 *      The number of byte exchange with the RAM, depending on the
 *      processor's word size and the reduced behavior bits in the
 *      status register.
 */
static int byteExchanged(const asrm* vm){
    switch((SR(vm) & 0x6) >> 1){
        case 0:
            return vm->config->word_size_byte;
        case 1:
            return min(vm->config->word_size_byte, 4);
        case 2:
            return min(vm->config->word_size_byte, 2);
        case 3:
            return 1;
        default: //We should never be there
            fprintf(stderr, "Error in byteExchanged\n");
            return 0;
    }
}

/*
 * Fetches a full word from ram
 *  Arguments:
 *      vm : the asrm struct
 *      addr : the address of the word to be fetched
 *  return:
 *      The word in RAM (the byte at the address and the following bytes)
 */
static word_t loadWordRAM(const asrm* vm, word_t addr){
    word_t ret = 0;
    for(int a=addr + byteExchanged(vm) - 1; a>=addr; a--){
        ret += vm->ram[a];
        if(a != addr)
            ret = ret << 8;
    }
    return ret;
}

/*
 * Put a whole word in RAM
 *  Arguments:
 *      vm : the asrm struct
 *      addr : the startin address of where we must put the word
 *      content : the word to be put in RAM
 */
static void putRAMWord(asrm* vm, word_t addr, word_t content){
    for(int offset=0; offset<byteExchanged(vm); offset++){
        uint8_t byteToSend = (uint8_t) (content >> (offset * 8)) & 0xFF;
        vm->ram[addr+offset] = byteToSend;
    }
}

/*
 * Check if a vm needs to do io.
 * If the address 0 in ram is 0, the content of address 1 is printed.
 * If the address 2 in ram is 0, a char is read and put in address 3.
 */
static void io(asrm* vm){
    if(!vm->ram[0]){
        printf("%c",vm->ram[1]);
        vm->ram[0] = 'A';
    }
    if(!vm->ram[2]){
        vm->ram[3] = getchar();
        vm->ram[2] = 'R';
    }
}

