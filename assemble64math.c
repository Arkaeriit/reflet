/*--------------------------------------------\
|This file contained functions used to compile|
|various mathematical opperation.             |
\--------------------------------------------*/

#include "assemble64math.h"

/*
 * compile a line of assembly language coding for an addition
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_add(char** elems,uint8_t n,uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(ADD_RR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RRR){
        a64_createMachineCode(ADD_RRR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RN){
        a64_createMachineCode(ADD_RN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for ADD operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for an substraction
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_sub(char** elems,uint8_t n,uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(SUB_RR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RRR){
        a64_createMachineCode(SUB_RRR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RN){
        a64_createMachineCode(SUB_RN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for SUB operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for an multiplication
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_tim(char** elems,uint8_t n,uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(TIM_RR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RRR){
        a64_createMachineCode(TIM_RRR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RN){
        a64_createMachineCode(TIM_RN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for TIM operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for an division
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_div(char** elems,uint8_t n,uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(DIV_RR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RRR){
        a64_createMachineCode(DIV_RRR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RN){
        a64_createMachineCode(DIV_RN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for DIV operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for a modulo operation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_mod(char** elems,uint8_t n,uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(MOD_RR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RRR){
        a64_createMachineCode(MOD_RRR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RN){
        a64_createMachineCode(MOD_RN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for MOD operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

