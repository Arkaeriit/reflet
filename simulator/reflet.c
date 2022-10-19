/*------------------------------------------\
|This file contain functions and definitions|
|used to make the simulator.                |
\------------------------------------------*/

#include "reflet.h"
static void run_inst(reflet* vm);
static int byteExchanged(const reflet* vm, bool stack_b);
static void check_alignement_ok(reflet* vm, word_t addr, bool stack_b);
static word_t loadWordRAM(reflet* vm, word_t addr, bool stack_b);
static void putRAMWord(reflet* vm, word_t addr, word_t content, bool stack_b);
static void io(reflet* vm);
static word_t new_int(reflet* vm, int interupt);
static void check_int(reflet* vm);
static void triger_int(reflet* vm, uint8_t int_number);
static void ret_int(reflet* vm);
static bool enabled_int(reflet* vm, int interupt);
#define min(x, y) (x < y ? x : y)

/*
 * Create a reflet struct with the content described in content.h
 *  return:
 *      A pointer to the struct
 */
reflet* reflet_init(){
    reflet* ret = malloc(sizeof(reflet));
    ret->ram = NULL; //Left blank, should be written a value depending of its configuration
    ret->active = true;
    ret->byte_mode = false;
    //default config
    struct reflet_config* conf = malloc(sizeof(struct reflet_config));
    conf->word_size = WORD_SIZE;
    conf->word_size_byte = WORD_SIZE_BYTE;
    conf->word_mask = WORD_MASK;
    conf->ram_size = RAM_SIZE;
    conf->tx_cmd = TX_CMD;
    conf->tx_data = TX_DATA;
    conf->rx_cmd = RX_CMD;
    conf->rx_data = RX_DATA;
    for(int i=0; i<4; i++){
        struct reflet_config_int* interupt = malloc(sizeof(struct reflet_config_int));
        interupt->enable = false;
        interupt->freq = 0;
        conf->ints[i] = interupt;
    }
    ret->config = conf;
    //default debug
    struct reflet_debug* debug = malloc(sizeof(struct reflet_debug));
    debug->enable = false;
    debug->steps = 1;
    ret->debug = debug;
    //Interruption data
    for(int i=0; i<4; i++){
        struct reflet_int* interupt = malloc(sizeof(struct reflet_int));
        interupt->routine = 0;
        interupt->count_up = 0;
        ret->ints[i] = interupt;
    }
    struct reflet_int_level* level = malloc(sizeof(struct reflet_int_level));
    level->level = LEVEL_NORMAL;
    level->stack_depth = 0;
    ret->int_level = level;
    //Input reset values not equal to 0
    ret->SR = 1;
    ret->PC = START_CHAR;
    return ret;
}

/*
 * Setup the ram of a Reflet struct depending of its configuration
 */
void reflet_initRAM(reflet* vm){
    vm->ram = calloc(vm->config->ram_size, sizeof(ram_word_t));
}

/*
 * Free a reflet struct
 */
void reflet_free(reflet* vm){
    for(int i=0; i<4; i++){
        free(vm->ints[i]);
        free(vm->config->ints[i]);
    }
    free(vm->int_level);
    free(vm->config);
    free(vm->ram);
    if(vm->debug->enable)
        fclose(vm->debug->file);
    free(vm->debug);
    free(vm);
}

/*
 * Run the program in the vm's RAM
 */
void reflet_run(reflet* vm){
    while((vm->PC < vm->config->ram_size) && vm->active){
        debugLog(vm);
        check_int(vm);
        run_inst(vm);
        io(vm);
    }
}

/*
 * Run a single instruction from the RAM
 */
static void run_inst(reflet* vm){
    word_t reg_mask = vm->config->word_mask;
    uint8_t instruction = (uint8_t) vm->ram[vm->PC];
    uint8_t opperand = (instruction & 0xF0) >> 4;
    uint8_t reg = instruction & 0x0F;
#if 0 //Toogle for easy debuging
    word_t debug_helper[16];
    for(int i=0; i<16; i++)
        debug_helper[i] = vm->reg[i];
    uint8_t* stack = vm->ram + vm->PC;
#endif
    bool edited_pc = false;
    switch(opperand){
        case 0:
            switch(instruction){
                case SLP:
                    break;
                case CC2:
                    vm->WR = ~vm->WR + 1;
                    break;
                case JIF:
                    if(vm->SR & 1) {
                        vm->PC = vm->WR;
                        edited_pc = true;
                    }
                    break;
                case POP:
                    vm->SP = (vm->SP - byteExchanged(vm, true)) & reg_mask;
                    vm->WR = loadWordRAM(vm, vm->SP, true);
                    break;
                case PUSH:
                    putRAMWord(vm, vm->SP, vm->WR, true);
                    vm->SP = (vm->SP + byteExchanged(vm, true)) & reg_mask;
                    break;
                case CALL:
                    putRAMWord(vm, vm->SP, vm->PC+1, true);
                    vm->SP = (vm->SP + byteExchanged(vm, true)) & reg_mask;
                    vm->PC = vm->WR;
                    edited_pc = true;
                    break;
                case RET:
                    vm->SP = (vm->SP - byteExchanged(vm, true)) & reg_mask;
                    vm->PC = loadWordRAM(vm, vm->SP, true);
                    edited_pc = true;
                    break;
                case QUIT:
                    vm->active = false;
                    break;
                case DEBUG:
                    printf("Debug instruction reached at address %" WORD_PX ". The content of the working register is 0x%" WORD_PX "\n", vm->PC, vm->WR);
                    break;
                case CMPNOT:
                    if(vm->SR & 1)
                        vm->SR = (vm->SR >> 1) << 1; //We want to turn the LSB into a 0
                    else
                        vm->SR |= 1;
                    break;
                case TBM:
                    vm->byte_mode = !vm->byte_mode;
                    break;
                default:
                    if(isSETINT(instruction)){
                        uint8_t int_number = instruction & 3;
                        vm->ints[int_number]->routine = vm->WR;
                    }else if(instruction == RETINT){
                        ret_int(vm);
                    }else{
                        fprintf(stderr, "Warning, unknow instruction (%X) at address %" WORD_P ".\n",instruction, vm->PC);
                    }
            }
            break;
        case SET:
            vm->WR = reg;
            break;
        case READ:
            vm->WR = vm->reg[reg];
            break;
        case CPY:
            vm->reg[reg] = vm->WR;
            if(reg == PC_REG)
                edited_pc = true;
            break;
        case ADD:
            vm->WR += vm->reg[reg];
            vm->WR &= reg_mask;
            break;
        case SUB:
            vm->WR -= vm->reg[reg];
            vm->WR &= reg_mask;
            break;
        case AND:
            vm->WR &= vm->reg[reg];
            break;
        case OR:
            vm->WR |= vm->reg[reg];
            break;
        case XOR:
            vm->WR ^= vm->reg[reg];
            break;
        case NOT:
            vm->WR = ~(vm->reg[reg]);
            break;
        case LSL:
            vm->WR = vm->WR << vm->reg[reg];
            vm->WR &= reg_mask;
            break;
        case LSR:
            vm->WR = vm->WR >> vm->reg[reg];
            vm->WR &= reg_mask;
            break;
        case EQ:
            if(vm->WR == vm->reg[reg]){
                vm->SR |= 1;
            }else{
                vm->SR = vm->SR >> 1;
                vm->SR = vm->SR << 1;
            }
            break;
        case LES:
            if(vm->WR < vm->reg[reg]){
                vm->SR |= 1;
            }else{
                vm->SR = vm->SR >> 1;
                vm->SR = vm->SR << 1;
            }
            break;
        case STR:
            putRAMWord(vm, vm->reg[reg], vm->WR, false);
            break;
        case LOAD:
            vm->WR = loadWordRAM(vm, vm->reg[reg], false);
            break;
        default:
            fprintf(stderr, "Error in the reading of the instruction at address %" WORD_P ".\n", vm->PC);
    }
    if(!edited_pc)
        vm->PC++;
}

/*
 * Compute how many byte of data the processor should
 * exchange with the RAM
 *  Argument :
 *      vm : the reflet struct
 *      stack_b : are we ignoring the behavior bits?
 *  return :
 *      The number of byte exchange with the RAM, depending on the
 *      processor's word size and the reduced behavior bits in the
 *      status register.
 */
static int byteExchanged(const reflet* vm, bool stack_b){
    if(stack_b)
        return vm->config->word_size_byte;
    if(vm->byte_mode && (vm->int_level->level == LEVEL_NORMAL))
        return 1;
    switch((vm->SR & 0x6) >> 1){
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
 * Check for invalid alignment in memory accesses. If the invalid alignment
 * trap is on, the interrupt is run. Otherwise, a message is printed.
 *  Arguments:
 *      vm : the reflet struct
 *      addr : the address of the memory to access
 *      stack_b : are we ignoring the behavior bits
 */
static void check_alignement_ok(reflet* vm, word_t addr, bool stack_b){
    uint8_t size = byteExchanged(vm, stack_b);
    word_t addr_invalid_mask = size-1;
    bool access_ok = !(addr & addr_invalid_mask);
    if(access_ok) return;
    bool trap_enabled = !(!(vm->SR & 0x88)); //Check that both the trap and the interupt 0 are enabled
    if(trap_enabled){
        if(vm->int_level->level != LEVEL_INT_0) //Roll back the PC so that after the interrupt, the program jumps back the the instruction that caused a problem
            vm->PC--;
        triger_int(vm, 0);
    }else{
        printf("Missaligned access at addr 0x%" WORD_PX ". Triying to acces %i bytes from address 0x%" WORD_PX ".\n", vm->PC-1, size, addr);
    }
}

/*
 * Fetches a full word from ram
 *  Arguments:
 *      vm : the reflet struct
 *      addr : the address of the word to be fetched
 *      stack_b : are we ignoring the behavior bits
 *  return:
 *      The word in RAM (the byte at the address and the following bytes)
 */
static word_t loadWordRAM(reflet* vm, word_t addr, bool stack_b){
    check_alignement_ok(vm, addr, stack_b);
    uint8_t* target = vm->ram + addr;
    int exchanged = byteExchanged(vm, stack_b);
    word_t ret = 0;
    for(int i=0; i<exchanged; i++){
        ret = ret << 8;
        ret += target[exchanged-i-1];
    }
    return ret;
}

/*
 * Put a whole word in RAM
 *  Arguments:
 *      vm : the reflet struct
 *      addr : the starting address of where we must put the word
 *      content : the word to be put in RAM
 *      stack_b : are we ignoring the behavior bits
 */
static void putRAMWord(reflet* vm, word_t addr, word_t content, bool stack_b){
    check_alignement_ok(vm, addr, stack_b);
    uint8_t* target = vm->ram + addr;
    for(int offset=0; offset<byteExchanged(vm, stack_b); offset++){
        uint8_t byteToSend = (uint8_t) (content >> (offset * 8)) & 0xFF;
        target[offset] = byteToSend;
    }
}

/*
 * Check if a vm needs to do io.
 * If the address 0 in ram is 0, the content of address 1 is printed.
 * If the address 2 in ram is 0, a char is read and put in address 3.
 */
static void io(reflet* vm){
    if(!vm->ram[vm->config->tx_cmd]){
        printf("%c",vm->ram[vm->config->tx_data]);
        vm->ram[vm->config->tx_cmd] = 'A';
    }
    if(!vm->ram[vm->config->rx_cmd]){
        char buff[2048]; //buffer to get a character
        fgets(buff, 2048, stdin);
        vm->ram[vm->config->rx_data] = buff[0];
        vm->ram[vm->config->rx_cmd] = 'R';
    }
}

/*
 * When a nex interrupt is activated, this function
 * update the stacks and returns the address of the interrupt routine
 *  Arguments:
 *      vm: the reflet struct
 *      interupt: the number of the interrupt being activated
 *  returns:
 *      The address of the routine to engage because of the interrupt
 */
static word_t new_int(reflet* vm, int interupt){
    word_t ret = vm->ints[interupt]->routine;
    vm->int_level->level_stack[vm->int_level->stack_depth] = vm->int_level->level;
    vm->int_level->routine_stack[vm->int_level->stack_depth] = vm->PC;
    vm->int_level->level = interupt;
    vm->int_level->stack_depth++;
    return ret;
}

/*
 * Check if we reached the count_up for any of the
 * interrupts and if so, update the level and the PC
 *  Arguments:
 *      vm: If you read the code, you know what it is
 */
static void check_int(reflet* vm){
    for(int i=0; i<4; i++) //increasing each timer
        vm->ints[i]->count_up++;
    for(int i=0; i<4; i++){
        if(vm->config->ints[i]->enable && vm->ints[i]->count_up >= vm->config->ints[i]->freq){ //Checking if an enabled interrupt reached its count_up
            vm->ints[i]->count_up = 0;
            triger_int(vm, i);
        }
    }
}

/*
 * Tries to trigger the desired interupt.
 */
static void triger_int(reflet* vm, uint8_t int_number){
    if(int_number < vm->int_level->level && enabled_int(vm, int_number)){ //The interrupt is taken into account if its priority is higher than the current level and it is enabled in the status register
        word_t routine = new_int(vm, int_number);
        vm->PC = routine;
    }
}

/*
 * Return from an interrupt and update the PC.
 * If there is no interrupts to return from, print a message
 * but do not do anything else.
 */
static void ret_int(reflet* vm){
    if(vm->int_level->stack_depth){ //We are in an interrupt context, everything is fine
        vm->int_level->stack_depth--;
        vm->int_level->level = vm->int_level->level_stack[vm->int_level->stack_depth];
        vm->PC = vm->int_level->routine_stack[vm->int_level->stack_depth];
    }else{
        fprintf(stderr, "Warning: using RETINT while not in an interrupt context.\n");
    }
}

/*
 * Check if the interrup the enabled in the status register
 */
static bool enabled_int(reflet* vm, int interupt){
    char flags = (vm->SR >> 3) & 0xF;
    bool ret = (flags >> interupt) & 1;
    return ret;
}

