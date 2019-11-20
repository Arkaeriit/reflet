#include "Exec64.h"

/*
 * Execute the code in a vm
 * Arguments:
 *      vm : the vm
 *  return:
 *      0 if everything is OK
 *      1 if an insruction can't be read
 */
int e64_execute(vm_64* vm){
    uint8_t reg1,reg2; //variable to store the registers' numbers.
    for(uint64_t i=0;i<vm->nombreInstruction;i++){
        uint64_t inst = vm->code[i];
        uint16_t opperand = inst & 0x3FF;
        //printf("%lx %x %x %lx\n",inst,opperand,e64_reg1(inst),e64_num1(inst)); //debug
        switch(opperand){
            case MOV_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = e64_num1(inst);
                break;
            case PRT_R :
                reg1 = e64_reg1(inst);
                printf("%" PRIu64 "\n",vm->registers[reg1]);
                break;
            default:
                fprintf(stderr,"Error, invalid Opperand : %" PRIx16 "\nInstruction : %" PRIx64 "\n",opperand,inst);
                return 1;
        }
    }
    return 0;
}
/*
 * Get the numeric argument of an instruction if the instruction take 1 regster as argument
 */
uint64_t e64_num1(uint64_t inst){
    return (inst & NUM1) >> 16;
}

/*
 * Return the 1rst register argument of an instruction.
 */
uint8_t e64_reg1(uint64_t inst){
    return (uint8_t) ((inst & REG1) >> 0);
}

