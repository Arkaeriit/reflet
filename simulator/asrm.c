/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#include "asrm.h"
static void run_inst(asrm* vm);
static int byteExchanged(const asrm* vm, bool stack_b);
static word_t loadWordRAM(const asrm* vm, word_t addr, bool stack_b);
static void putRAMWord(asrm* vm, word_t addr, word_t content, bool stack_b);
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
    ret->active = true;
    //default config
    struct asrm_config* conf = malloc(sizeof(struct asrm_config));
    conf->word_size = WORD_SIZE;
    conf->word_size_byte = WORD_SIZE_BYTE;
    conf->word_mask = WORD_MASK;
    conf->ram_size = RAM_SIZE;
    conf->tx_cmd = TX_CMD;
    conf->tx_data = TX_DATA;
    conf->rx_cmd = RX_CMD;
    conf->rx_data = RX_DATA;
    ret->config = conf;
    //default debug
    struct asrm_debug* debug = malloc(sizeof(struct asrm_debug));
    debug->enable = false;
    debug->steps = 1;
    ret->debug = debug;
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
    if(vm->debug->enable)
        fclose(vm->debug->file);
    free(vm->debug);
    free(vm);
}

/*
 * Run the program in the vm's RAM
 */
void asrm_run(asrm* vm){
    while((PC(vm) < vm->config->ram_size) && vm->active){
        debugLog(vm);
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
            if(instruction == SLP){
                break;
            }else if(instruction == JMP){
                PC(vm) = WR(vm);
            }else if(instruction == JIF){
                if(SR(vm) & 1)
                    PC(vm) = WR(vm);
            }else if(instruction == POP){
                SP(vm) = (SP(vm) - 1) & reg_mask;
                WR(vm) = loadWordRAM(vm, SP(vm), true);
            }else if(instruction == PUSH){
                putRAMWord(vm, SP(vm), WR(vm), true);
                SP(vm) = (SP(vm) + 1) & reg_mask;
            }else if(instruction == CALL){
                putRAMWord(vm, SP(vm), PC(vm), true); //-1 because we already updated the PC and we want to store the address of the call
                SP(vm) = (SP(vm) + 1) & reg_mask;
                PC(vm) = WR(vm);
            }else if(instruction == RET){
                SP(vm) = (SP(vm) - 1) & reg_mask;
                PC(vm) = loadWordRAM(vm, SP(vm), true);
            }else if(instruction == QUIT){
                vm->active = false;
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
            putRAMWord(vm, vm->reg[reg], WR(vm), false);
            break;
        case LOAD:
            WR(vm) = loadWordRAM(vm, vm->reg[reg], false);
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
 *      stack_b : are we ignoring the behavior bits?
 *  return :
 *      The number of byte exchange with the RAM, depending on the
 *      processor's word size and the reduced behavior bits in the
 *      status register.
 */
static int byteExchanged(const asrm* vm, bool stack_b){
    if(stack_b)
        return vm->config->word_size_byte;
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
 *      stack_b : are we ignoring the behavior bits
 *  return:
 *      The word in RAM (the byte at the address and the following bytes)
 */
static word_t loadWordRAM(const asrm* vm, word_t addr, bool stack_b){
    word_t ret = 0;
    for(int a=addr + byteExchanged(vm, stack_b) - 1; a>=addr; a--){
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
 *      stack_b : are we ignoring the behavior bits
 */
static void putRAMWord(asrm* vm, word_t addr, word_t content, bool stack_b){
    for(int offset=0; offset<byteExchanged(vm, stack_b); offset++){
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
    if(!vm->ram[vm->config->tx_cmd]){
        printf("%c",vm->ram[vm->config->tx_data]);
        vm->ram[vm->config->tx_cmd] = 'A';
    }
    if(!vm->ram[vm->config->rx_cmd]){
        vm->ram[vm->config->rx_data] = getchar();
        vm->ram[vm->config->rx_cmd] = 'R';
    }
}

