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
    uint64_t i = 0; //Index of the position of the line of machine code that must be read
    while(i < vm->nombreInstruction){
        uint64_t inst = vm->code[i];
        uint16_t opperand = inst & OPP;
        //printf("%lx %x %x %lx\n",inst,opperand,e64_reg1(inst),e64_num1(inst)); //debug
        switch(opperand){
            case NIL :
                break;
            case QUIT :
                return e64_num0(inst);
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
            case SUB_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] -= e64_num1(inst);
                break;
            case SUB_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] -= vm->registers[reg2];
                break;
            case SUB_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] - vm->registers[reg3];
                break;
            case TIM_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] *= e64_num1(inst);
                break;
            case TIM_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] *= vm->registers[reg2];
                break;
            case TIM_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] * vm->registers[reg3];
                break;
            case DIV_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] /= e64_num1(inst);
                break;
            case DIV_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] /= vm->registers[reg2];
                break;
            case DIV_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] / vm->registers[reg3];
                break;
            case MOD_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] %= e64_num1(inst);
                break;
            case MOD_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] %= vm->registers[reg2];
                break;
            case MOD_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] % vm->registers[reg3];
                break;
            case AND_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] &= e64_num1(inst);
                break;
            case AND_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] &= vm->registers[reg2];
                break;
            case AND_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] & vm->registers[reg3];
                break;
            case OR_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] |= e64_num1(inst);
                break;
            case OR_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] |= vm->registers[reg2];
                break;
            case OR_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] | vm->registers[reg3];
                break;
            case XOR_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] ^= e64_num1(inst);
                break;
            case XOR_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] ^= vm->registers[reg2];
                break;
            case XOR_RRR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                reg3 = e64_reg3(inst);           
                vm->registers[reg1] = vm->registers[reg2] ^ vm->registers[reg3];
                break;
            case NOT_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = !e64_num1(inst);
                break;
            case NOT_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] = !vm->registers[reg2];
                break;
            case NOT_R :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = !vm->registers[reg1];
                break;
            case CMP_RN :
                reg1 = e64_reg1(inst);
                e64_cmp(vm->flags, vm->registers[reg1], e64_num1(inst));
                break;
            case CMP_RR : 
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                e64_cmp(vm->flags, vm->registers[reg1], vm->registers[reg2]);
                break;
            case PUSH :
                reg1 = e64_reg1(inst);
                st_push(vm->registersStack, vm->registers[reg1]);
                break;
            case POP :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = st_pull(vm->registersStack);
                break;
            case FREE :
                reg1 = e64_reg1(inst);
                free((uint64_t*) vm->registers[reg1]);
                break;
            case ASK_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] = (uint64_t) malloc(sizeof(uint64_t) * vm->registers[reg2]);
                break;
            case ASK_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = (uint64_t) malloc(sizeof(uint64_t) * e64_num1(inst));
                break;
            case ASK_BYTE_RR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] = (uint64_t) malloc(vm->registers[reg2]);
                break;
            case ASK_BYTE_RN :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = (uint64_t) malloc(e64_num1(inst));
                break;
            case STR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                *((uint64_t*) vm->registers[reg1]) = vm->registers[reg2];
                break;
            case LDR :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] = *((uint64_t*) vm->registers[reg2]);
                break;
            case STR_BYTE :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                *((uint64_t*) vm->registers[reg1]) = vm->registers[reg2] & 0xFF;
                break;
            case LDR_BYTE :
                reg1 = e64_reg1(inst);
                reg2 = e64_reg2(inst);
                vm->registers[reg1] = *((uint64_t*) vm->registers[reg2]);
                vm->registers[reg2] &= 0xFF;
                break;
            case LDR_DATA :
                reg1 = e64_reg1(inst);
                vm->registers[reg1] = (uint64_t) vm->data[e64_num1(inst)];
                break;
            case PCHAR :
                reg1 = e64_reg1(inst);
                putchar(vm->registers[reg1]);
                break;
            case SPRINT :
                reg1 = e64_reg1(inst);
                printf("%s", (char*) vm->registers[reg1]);
                break;
            case DSP_R :
                reg1 = e64_reg1(inst);
                printf("%" PRIu64 "\n",vm->registers[reg1]);
                break;
            case JMP : 
                i = e64_num0(inst); /*The jump pu us in the place we were after the label. The i-- compensates the i++ and will put us where we want to be.*/
                i--;
                break;
            case JZ :
                if(vm->flags[FLAG_ZERO]){
                    i = e64_num0(inst);
                    i--;
                }
                break;
            case JNZ :
                if(!vm->flags[FLAG_ZERO]){
                    i = e64_num0(inst);
                    i--;
                }
                break;
            case JB :
                if(vm->flags[FLAG_BIGGER]){
                    i = e64_num0(inst);
                    i--;
                }
                break;
            case JBE :
                if(!vm->flags[FLAG_SMALLER]){
                    i = e64_num0(inst);
                    i--;
                }
                break;
            case JS :
                if(vm->flags[FLAG_SMALLER]){
                    i = e64_num0(inst);
                    i--;
                }
                break;
            case JSE :
                if(!vm->flags[FLAG_BIGGER]){
                    i = e64_num0(inst);
                    i--;
                }
                break;
            case CALL :
                st_push(vm->functionsStack, i);
                i = e64_num0(inst);
                i--;
                break;
            case RET :
                i = st_pull(vm->functionsStack); /*There is no i-- here because we saved the posithion from where we made th function call so we need the i++ to move on*/
                break;
            default:
                fprintf(stderr,"Error, invalid Opperand : %" PRIx16 "\nInstruction : %" PRIx64 "\n",opperand,inst);
                return EXECUTE_UNKNOWN_OPPERAND;
        }
        i++;
    }
    return EXECUTE_OK;
}

/*
 * Get the numeric argument of an instruction if the instruction take 0 regster as argument
 */
uint64_t e64_num0(uint64_t inst){
    return (inst & NUM0) >> ARG_SHIFT_1;
}

/*
 * Get the numeric argument of an instruction if the instruction take 1 regster as argument
 */
uint64_t e64_num1(uint64_t inst){
    return (inst & NUM1) >> ARG_SHIFT_2;
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

/*
 * Update the flags of the VM when comparing two numbers
 *  Arguments : 
 *      flags : a pointer to the flags
 *      num1 : first number to compare
 *      num2 : second number to compare
 */
void e64_cmp(bool* flags, uint64_t num1, uint64_t num2){
    flags[FLAG_ZERO] = (num1 == num2);
    flags[FLAG_BIGGER] = (num1 > num2);
    flags[FLAG_SMALLER] = (num1 < num2);
}
   
