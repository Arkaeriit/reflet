/*---------------------------------------------------------*\
|This file contain what is needed to execute the code loaded|
|into a vm.                                                 |
\----------------------------------------------------------*/

#include "Exec64.h"

/*
 * Execute the code in a vm
 * Arguments:
 *      vm : the vm
 *  return:
 *      EXECUTE_OK if everything is OK
 *      an error code if an insruction can't be read
 */
int e64_execute(vm_64* vm){
    uint8_t reg1,reg2,reg3; //variable to store the registers' numbers.
    for(uint64_t i=0;i<vm->nombreInstruction;i++){
        uint64_t inst = vm->code[i];
        uint16_t opperand = inst & 0x3FF;
        //printf("%lx %x %x %lx\n",inst,opperand,e64_reg1(inst),e64_num1(inst)); //debug
        switch(opperand){
            case MOV_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = e64_num1(inst);
                break;
            case MOV_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] = vm->registers[reg2];
                break;
            case ADD_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] += e64_num1(inst);
                break;
            case ADD_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] += vm->registers[reg2];
                break;
            case ADD_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] + vm->registers[reg3];
                break;
            case DSP_R :
                reg1 = e64_reg1(inst);
                printf("%" PRIu64 "\n",vm->registers[reg1]);
                break;
            default:
                fprintf(stderr,"Error, invalid Opperand : %" PRIx16 "\nInstruction : %" PRIx64 "\n",opperand,inst);
                return EXECUTE_UNKNOWN_OPPERAND;
        }
    }
    return EXECUTE_OK;
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
    return (uint8_t) ((inst & REG1) >> ARG_SHIFT_1);
}

/*
 * Return the 2nd register argument of an instruction
 */
uint8_t e64_reg2(uint64_t inst){
    return (uint8_t) ((inst & REG2) >> ARG_SHIFT_2);
}

/*
 * Return the 3rd register argument of an instruction
 */
uint8_t e64_reg3(uint64_t inst){
    return (uint8_t) ((inst & REG3) >> ARG_SHIFT_3);
}

