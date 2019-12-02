#include "assemble64memory.h"

uint8_t a64m_free(char** elems, uint8_t n, uint64_t* fullCode){
    if(n != 2 || elems[1][0] != 'R'){
        fprintf(stderr,"    Wrong argument for FREE operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    uint8_t reg1 = aXX_readDec(elems[1] + 1);
    *fullCode = FREE;
    *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
    return COMPILED_LINE_INSTRUCTION;
}

uint8_t a64m_ask(char** elems, uint8_t n, uint64_t* fullCode){
    if(n != 3 || elems[1][0] != 'R'){
        fprintf(stderr,"    Wrong argument for FREE operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    uint8_t reg1 = aXX_readDec(elems[1] + 1);
    if(elems[2][0] == 'R'){
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = ASK_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
    }else if(elems[2][0] == 'X'){
        uint64_t num1 = aXX_readHex(elems[2] + 1);
        *fullCode = ASK_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num1 & NUM1_MASK) << ARG_SHIFT_2;
    }else{
        uint64_t num1 = aXX_readDec(elems[2]);
        *fullCode = ASK_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num1 & NUM1_MASK) << ARG_SHIFT_2;
    }
    return COMPILED_LINE_INSTRUCTION;
}

uint8_t a64m_ask_byte(char** elems, uint8_t n, uint64_t* fullCode){
    if(n != 3 || elems[1][0] != 'R'){
        fprintf(stderr,"    Wrong argument for FREE operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    uint8_t reg1 = aXX_readDec(elems[1] + 1);
    if(elems[2][0] == 'R'){
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = ASK_BYTE_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
    }else if(elems[2][0] == 'X'){
        uint64_t num1 = aXX_readHex(elems[2] + 1);
        *fullCode = ASK_BYTE_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num1 & NUM1_MASK) << ARG_SHIFT_2;
    }else{
        uint64_t num1 = aXX_readDec(elems[2]);
        *fullCode = ASK_BYTE_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num1 & NUM1_MASK) << ARG_SHIFT_2;
    }
    return COMPILED_LINE_INSTRUCTION;
}

