
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
    if(n == 3 && elems[1][0] == 'R' && elems[2][0] != 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint64_t num;
        if(elems[2][1] == 'X')
            num = aXX_readHex(elems[2] + 1);
        else
            num = aXX_readDec(elems[2]);
        *fullCode = ADD_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num & NUM1_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 3 && elems[1][0] == 'R' && elems[2][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = ADD_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 4 && elems[1][0] == 'R' && elems[2][0] == 'R' && elems[3][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        uint8_t reg3 = aXX_readDec(elems[3] + 1);
        *fullCode = ADD_RRR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        *fullCode |= (reg3 & REG_MASK) << ARG_SHIFT_3;
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
    if(n == 3 && elems[1][0] == 'R' && elems[2][0] != 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint64_t num;
        if(elems[2][1] == 'X')
            num = aXX_readHex(elems[2] + 1);
        else
            num = aXX_readDec(elems[2]);
        *fullCode = SUB_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num & NUM1_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 3 && elems[1][0] == 'R' && elems[2][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = SUB_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 4 && elems[1][0] == 'R' && elems[2][0] == 'R' && elems[3][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        uint8_t reg3 = aXX_readDec(elems[3] + 1);
        *fullCode = SUB_RRR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        *fullCode |= (reg3 & REG_MASK) << ARG_SHIFT_3;
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
    if(n == 3 && elems[1][0] == 'R' && elems[2][0] != 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint64_t num;
        if(elems[2][1] == 'X')
            num = aXX_readHex(elems[2] + 1);
        else
            num = aXX_readDec(elems[2]);
        *fullCode = TIM_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num & NUM1_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 3 && elems[1][0] == 'R' && elems[2][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = TIM_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 4 && elems[1][0] == 'R' && elems[2][0] == 'R' && elems[3][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        uint8_t reg3 = aXX_readDec(elems[3] + 1);
        *fullCode = TIM_RRR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        *fullCode |= (reg3 & REG_MASK) << ARG_SHIFT_3;
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
    if(n == 3 && elems[1][0] == 'R' && elems[2][0] != 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint64_t num;
        if(elems[2][1] == 'X')
            num = aXX_readHex(elems[2] + 1);
        else
            num = aXX_readDec(elems[2]);
        *fullCode = DIV_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num & NUM1_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 3 && elems[1][0] == 'R' && elems[2][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = DIV_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 4 && elems[1][0] == 'R' && elems[2][0] == 'R' && elems[3][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        uint8_t reg3 = aXX_readDec(elems[3] + 1);
        *fullCode = DIV_RRR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        *fullCode |= (reg3 & REG_MASK) << ARG_SHIFT_3;
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
    if(n == 3 && elems[1][0] == 'R' && elems[2][0] != 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint64_t num;
        if(elems[2][1] == 'X')
            num = aXX_readHex(elems[2] + 1);
        else
            num = aXX_readDec(elems[2]);
        *fullCode = MOD_RN;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (num & NUM1_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 3 && elems[1][0] == 'R' && elems[2][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = MOD_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(n == 4 && elems[1][0] == 'R' && elems[2][0] == 'R' && elems[3][0] == 'R'){
        uint8_t reg1 = aXX_readDec(elems[1] + 1);
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        uint8_t reg3 = aXX_readDec(elems[3] + 1);
        *fullCode = MOD_RRR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        *fullCode |= (reg3 & REG_MASK) << ARG_SHIFT_3;
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for MOD operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

