/*------------------------------------------------------\
|This file contain function used to compile instructions|
|related to branching and function call.                |
\------------------------------------------------------*/

#include "assemble64jump.h"

/*
 * compile a line of assembly language coding for a comparaison
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_INSTRUCTION if the compilation of the line went well
 *      COMPILED_LINE_NOT_OK if there is an error
 */
uint8_t a64j_cmp(char** elems,uint8_t n,uint64_t* fullCode){
    if(n != 3 || elems[1][0] != 'R'){        
        fprintf(stderr,"    Wrong argument for JMP operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    uint8_t reg1 = aXX_readDec(elems[1] + 1);
    uint64_t num;
    if(elems[2][0] == 'R'){ //two registers
        uint8_t reg2 = aXX_readDec(elems[2] + 1);
        *fullCode = CMP_RR;
        *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
        *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
        return COMPILED_LINE_INSTRUCTION;
    }else if(elems[2][0] == 'X'){ //exadecimal number
        num = aXX_readHex(elems[2] + 1);
    }else{
        num = aXX_readDec(elems[2]);
    }
    *fullCode = CMP_RN;
    *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
    *fullCode |= (num & NUM1_MASK) << ARG_SHIFT_2;
    return COMPILED_LINE_INSTRUCTION;
}

/*
 * compile a line of assembly language coding for a label
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_BRANCH if the compilation of the line went well
 *      COMPILED_LINE_NOT_OK if there is an error
 */
uint8_t a64j_lab(char** elems,uint8_t n,char* ret){
    if(n != 2){
        fprintf(stderr,"    Wrong argument for marking a label.\n");
        return COMPILED_LINE_NOT_OK;
    }
    *ret = 'd';
    strcpy(ret + 3,elems[1]); //We add the name of the label 3 bytes into the instruction
    return COMPILED_LINE_BRANCH;
}

/*
 * compile a line of assembly language coding for a jump
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_BRANCH if the compilation of the line went well
 *      COMPILED_LINE_NOT_OK if there is an error
 */
uint8_t a64j_jmp(char** elems,uint8_t n,char* ret){
    if(n != 2){
        fprintf(stderr,"    Wrong argument for JMP operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    *ret = 'j';
    *((uint16_t*) (ret+1)) = JMP; //We define the opperand
    strcpy(ret + 3,elems[1]); //We add the name of the label 3 bytes into the instruction
    return COMPILED_LINE_BRANCH;
}

/*
 * compile a line of assembly language coding for a conditional jump about equality
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_BRANCH if the compilation of the line went well
 *      COMPILED_LINE_NOT_OK if there is an error
 */
uint8_t a64j_jz(char** elems,uint8_t n,char* ret){
    if(n != 2){
        fprintf(stderr,"    Wrong argument for JMP operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    *ret = 'j';
    *((uint16_t*) (ret+1)) = JZ; //We define the opperand
    strcpy(ret + 3,elems[1]); //We add the name of the label 3 bytes into the instruction
    return COMPILED_LINE_BRANCH;
}

/*
 * compile a line of assembly language coding for a conditional jump (if x>y)
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_BRANCH if the compilation of the line went well
 *      COMPILED_LINE_NOT_OK if there is an error
 */
uint8_t a64j_jb(char** elems,uint8_t n,char* ret){
    if(n != 2){
        fprintf(stderr,"    Wrong argument for JMP operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    *ret = 'j';
    *((uint16_t*) (ret+1)) = JB; //We define the opperand
    strcpy(ret + 3,elems[1]); //We add the name of the label 3 bytes into the instruction
    return COMPILED_LINE_BRANCH;
}

/*
 * compile a line of assembly language coding for a conditional jump (x<=y)
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_BRANCH if the compilation of the line went well
 *      COMPILED_LINE_NOT_OK if there is an error
 */
uint8_t a64j_jse(char** elems,uint8_t n,char* ret){
    if(n != 2){
        fprintf(stderr,"    Wrong argument for JMP operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    *ret = 'j';
    *((uint16_t*) (ret+1)) = JSE; //We define the opperand
    strcpy(ret + 3,elems[1]); //We add the name of the label 3 bytes into the instruction
    return COMPILED_LINE_BRANCH;
}

