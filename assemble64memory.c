/*----------------------------------------------------\
|This file contain function to compile code related to|
|memory allocation.                                   |
\----------------------------------------------------*/

#include "assemble64memory.h"

/*
 * compile a line of assembly language coding for a free opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_free(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_R){
        a64_createMachineCode(FREE, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for FREE operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for a ask opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_ask(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(ASK_RR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RN){
        a64_createMachineCode(ASK_RN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for ASK operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for a ask_byte opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_ask_byte(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(ASK_BYTE_RR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(config == COMPILE_RN){
        a64_createMachineCode(ASK_BYTE_RN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for ASK_BYTE operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

