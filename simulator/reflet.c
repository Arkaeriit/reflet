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
static void prepare_extended_io(reflet* vm);
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
    memset(ret->reg, 0, sizeof(word_t) * 16);
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
    conf->extended_io = false;
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
    ret->int_level = level;
    //Input reset values not equal to 0
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
    if (vm->config->extended_io) {
        prepare_extended_io(vm);
    }
    while (vm->active) {
        debugLog(vm);
        check_int(vm);
        run_inst(vm);
        io(vm);
        if (vm->PC >= vm->config->ram_size) {
            fprintf(stderr, "Error: the program counter (0x%" WORD_PX ") is greater than the highest address available.\n", vm->PC);
            exit(RET_MEMORY);
        }
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
        case 0xE:
            switch(instruction){
                case CC2:
                    vm->WR = ~vm->WR + 1;
                    vm->WR &= reg_mask;
                    break;
                case JIF:
                    if(vm->SR & SR_COMP_MASK) {
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
                    if(vm->SR & SR_COMP_MASK)
                        vm->SR &= ~SR_COMP_MASK; //We want to turn the LSB into a 0
                    else
                        vm->SR |= SR_COMP_MASK;
                    break;
                case TBM:
                    vm->byte_mode = !vm->byte_mode;
                    break;
                case ATOM: {
                    word_t currentValue = loadWordRAM(vm, vm->WR, false);
                    putRAMWord(vm, vm->WR, 1, false);
                    if(currentValue == 0){
                        vm->SR |= SR_COMP_MASK;
                    }else{
                        vm->SR &= ~SR_COMP_MASK;
                    }
                    } break;
                default:
                    if(isSETINT(instruction)){
                        uint8_t int_number = instruction & 3;
                        vm->ints[int_number]->routine = vm->WR;
                    }else if(instruction == RETINT){
                        ret_int(vm);
                        edited_pc = true;
                    }else{
                        fprintf(stderr, "Warning, unknow instruction (%X) at address %" WORD_P ".\n",instruction, vm->PC);
                    }
            }
            break;
        case 0xF: {
            uint8_t int_number = instruction & 3;
            if (isGETINT(instruction)) {
                vm->WR = vm->ints[int_number]->routine;
            } else if (isGETINTSTACK(instruction)) {
                vm->WR = vm->int_level->routine_stack[int_number];
            } else if (isSETINTSTACK(instruction)) {
                vm->int_level->routine_stack[int_number] = vm->WR;
            } else if (isSOFTINT(instruction)) {
                int previous_level = vm->int_level->level;
                vm->PC++; // We do that so we don't fall into an infinite loop after leaving the instructions.
                triger_int(vm, int_number);
                if (vm->int_level->level == previous_level) { // No new int
                    vm->PC--;
                } else {
                    edited_pc = true; // We don't want to edit the PC because we don't want to miss the first instruction of the interrupt routine.
                }
            } else {
                fprintf(stderr, "Warning, unknow instruction (%X) at address %" WORD_P ".\n",instruction, vm->PC);
            }
            } break;
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
            vm->WR &= reg_mask;
            break;
        case LSL:
            vm->WR = vm->WR << vm->reg[reg];
            vm->WR &= reg_mask;
            break;
        case LSR:
            vm->WR = vm->WR >> vm->reg[reg];
            break;
        case EQ:
            if(vm->WR == vm->reg[reg]){
                vm->SR |= SR_COMP_MASK;
            }else{
                vm->SR &= ~SR_COMP_MASK;
            }
            break;
        case LES:
            if(vm->WR < vm->reg[reg]){
                vm->SR |= SR_COMP_MASK;
            }else{
                vm->SR &= ~SR_COMP_MASK;
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
    int reduced_behavior_bits = vm->SR >> SR_REDUCED_POS;
    if (reduced_behavior_bits == 0)
        return vm->config->word_size_byte;
    else
        return 1 << (reduced_behavior_bits - 1);
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
    bool trap_enabled = (vm->SR & SR_INT0_MASK) && (vm->SR & SR_TRAP_MASK); //Check that both the trap and the interupt 0 are enabled
    if(trap_enabled && vm->int_level->level != LEVEL_INT_0) {
        vm->PC--; //Roll back the PC so that after the interrupt, the program jumps back the the instruction that caused a problem
        triger_int(vm, 0);
        vm->PC--; // The instruction will continue and update the PC at the end. We still want to execute the start of the interrupt routine so we must go back an instruction.
    }else{
        printf("Missaligned access at addr 0x%" WORD_PX ". Triying to acces %i bytes from address 0x%" WORD_PX ".\n", vm->PC-1, size, addr);
    }
}

/*
 * Check if the current memory access is inside of the allocated RAM.
 * If it is not, print it and exit the program.
 *  Arguments:
 *      vm : the reflet struct
 *      addr : the starting address of where we must put the word
 *      stack_b : are we ignoring the behavior bits
 */
static void check_access_valid(const reflet* vm, word_t addr, bool stack_b) {
    int byte_exchanged = byteExchanged(vm, stack_b);
    if (addr + byte_exchanged > vm->config->ram_size) {
        fprintf(stderr, "Error: the vm tries to access address 0x%" WORD_PX " while having only access to address 0x%" WORD_PX"!\n", addr + byte_exchanged - 1, vm->config->ram_size - 1);
        exit(RET_MEMORY);
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
    check_access_valid(vm, addr, stack_b);
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
    check_access_valid(vm, addr, stack_b);
    check_alignement_ok(vm, addr, stack_b);
    uint8_t* target = vm->ram + addr;
    for(int offset=0; offset<byteExchanged(vm, stack_b); offset++){
        uint8_t byteToSend = (uint8_t) (content >> (offset * 8)) & 0xFF;
        target[offset] = byteToSend;
    }
}

/*
 * If the address tx_cmd in ram is 0, the content of address tx_data is printed
 * If the address rx_cmd in ram is 0, a char is read and put in address rx_data
 * For read, the rx_cmd is filled back with a status value of 1 in success and
 * 2 in failure.
 */
static void basic_io(reflet* vm){
    if(!vm->ram[vm->config->tx_cmd]){
        printf("%c",vm->ram[vm->config->tx_data]);
        vm->ram[vm->config->tx_cmd] = 'A';
    }
    if(!vm->ram[vm->config->rx_cmd]){
        int ch = getchar();
        vm->ram[vm->config->rx_data] = (char) ch;
        vm->ram[vm->config->rx_cmd] = ch == EOF ? 2 : 1;
    }
}

enum {
    IO_NOP = 0,
    IO_WRITE = 1,
    IO_READ = 2,
} extended_io_cmds;

#define IO_COUT   0
#define IO_CIN    1
#define IO_CMD    2
#define IO_STATUS 3

/*
 * Perform extended IO opperations.
 */
static void extended_io(reflet* vm) {
    switch (vm->ram[IO_CMD]) {
        case IO_WRITE:
            putchar(vm->ram[IO_COUT]);
            break;
        case IO_READ: {
            int ch = getchar();
            vm->ram[IO_CIN] = (char) ch;
            vm->ram[IO_STATUS] = ch == EOF ? 1 : 0;}
            break;
        case IO_NOP:
        default:
            break;
    }
    vm->ram[IO_CMD] = IO_NOP;
}

/*
 * Force a nop in io cmd to ensure no unfortunate IO op happen when starting
 * the simulator.
 */
static void prepare_extended_io(reflet* vm) {
    vm->ram[IO_CMD] = IO_NOP;
}

/*
 * Run IO routine dicted by configuration.
 */
static void io(reflet* vm) {
    if (vm->config->extended_io) {
        extended_io(vm);
    } else {
        basic_io(vm);
    }
}

/*
 * When a new interrupt is activated, this function
 * update the stacks and returns the address of the interrupt routine
 *  Arguments:
 *      vm: the reflet struct
 *      interupt: the number of the interrupt being activated
 *  returns:
 *      The address of the routine to engage because of the interrupt
 */
static word_t new_int(reflet* vm, int interupt){
    word_t ret = vm->ints[interupt]->routine;
    vm->int_level->level_stack[interupt] = vm->int_level->level;
    vm->int_level->routine_stack[interupt] = vm->PC;
    vm->int_level->level = interupt;
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
    if(vm->int_level->level != LEVEL_NORMAL){ //We are in an interrupt context, everything is fine
        vm->PC = vm->int_level->routine_stack[vm->int_level->level];
        vm->int_level->level = vm->int_level->level_stack[vm->int_level->level];
    }else{
        fprintf(stderr, "Warning: using RETINT while not in an interrupt context.\n");
    }
}

/*
 * Check if the interrup the enabled in the status register
 */
static bool enabled_int(reflet* vm, int interupt){
    switch (interupt) {
        case 0:
            return vm->SR & SR_INT0_MASK;
        case 1:
            return vm->SR & SR_INT1_MASK;
        case 2:
            return vm->SR & SR_INT2_MASK;
        case 3:
            return vm->SR & SR_INT3_MASK;
        default:
            fprintf(stderr, "Error, invalid interrupt number: %i\n", interupt);
            return false;
    }
}

