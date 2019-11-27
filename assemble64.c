/*----------------------------------------------------------------\
|This file contant what is needed to precompile assembly code into|
|a object file for 64 bits machine code.                          |
\----------------------------------------------------------------*/

#include "assemble64.h"

/*
 * This function take a file containing assembly code for the asvm
 * and compile it insto an asvm object file
 * Arguments :
 *      fin : pointer to the assembly code file
 *      fout : pointer to the object file
 */
void a64_assemble(FILE* fin,FILE* fout){
    OBJECT_MW_TYPE magicWord = OBJECT_MW; //Writing the magic word
    fwrite(&magicWord, 1, OBJECT_MW_SIZE , fout);
    char* line = malloc(SIZELINE);
    uint64_t lineNumber = 1;
    char** elems = malloc(sizeof(char*) * 150); //We will never get 150 words
    fgets(line,255,fin);
    while(line != NULL && strlen(line) > 2){
        uint8_t n = aXX_preprocessLine(line,elems);
        uint8_t a = a64_compileLine(elems,n,line);
        if(a == COMPILED_LINE_INSTRUCTION){
            fprintf(fout,"i");
            fwrite(line,8,1,fout);
        }else if(a == COMPILED_LINE_BRANCH){
            fprintf(fout,"j");
            fwrite(line,200,1,fout);
        }else{
            fprintf(stderr,"Error line %" PRIx64 ":\n",lineNumber);
            return;
        }
        
        aXX_freeElems(n,elems);
        fgets(line,SIZELINE,fin);
        lineNumber++;
    } 
}

/*
 * compile a line of assembly language
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      ret : the compiled code
 * Return :
 *      COMPILED_LINE_NOT_OK if the result is a compiled instruction
 *      COMPILED_LINE_BRANCH if the result is about branching
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64_compileLine(char** elems,uint8_t n,char* ret){
    memset(ret,0,strlen(ret));
    uint64_t* fullCode = (uint64_t*) ret; //representing the output as a 64 bit number
    if(!strcmp(elems[0],"MOV") && n == 3){
        if(elems[1][0] == 'R'){
            if(elems[2][0] == 'R'){ //A register
                uint8_t reg1 = aXX_readDec(elems[1] + 1); //The +1 is used to prevent readDec from reading the R
                uint8_t reg2 = aXX_readDec(elems[2] + 1);
                *fullCode = MOV_RR;
                *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1; //We set the result register
                *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2; //We set the input register
                return COMPILED_LINE_INSTRUCTION;
            }else if(elems[2][0] == 'X'){ //An hexadecimal number
                uint8_t reg1 = aXX_readDec(elems[1] + 1);
                uint64_t num = aXX_readHex(elems[2] + 1);
                *fullCode = MOV_RN; //We set the opperand
                *fullCode |= (reg1 & REG_MASK ) << ARG_SHIFT_1; //We set the register
                *fullCode |= (num & NUM1_MASK ) << ARG_SHIFT_2; //We set the data
                return COMPILED_LINE_INSTRUCTION;
            }else{ //A decimal number
                uint8_t reg1 = aXX_readDec(elems[1] + 1);
                uint64_t num = aXX_readDec(elems[2]);
                *fullCode = MOV_RN; //We set the opperand
                *fullCode |= (reg1 & REG_MASK ) << ARG_SHIFT_1 ; //We set the register
                *fullCode |= (num & NUM1_MASK ) << ARG_SHIFT_2 ; //We set the data
                return COMPILED_LINE_INSTRUCTION;
            }
        }else{
            fprintf(stderr,"    Wrong argument for MOV operation.\n");
            return COMPILED_LINE_NOT_OK;
        }
    }else if(!strcmp(elems[0],"ADD")){
        return a64m_add(elems, n, fullCode);
    }else if(!strcmp(elems[0],"SUB")){
        return a64m_sub(elems, n, fullCode);
    }else if(!strcmp(elems[0],"TIM")){
        return a64m_tim(elems, n, fullCode);
    }else if(!strcmp(elems[0],"DIV")){
        return a64m_div(elems, n, fullCode);
    }else if(!strcmp(elems[0],"MOD")){
        return a64m_mod(elems, n, fullCode);
    }else if(!strcmp(elems[0],"DSP") && n == 2){
        if(elems[1][0] == 'R'){
            uint8_t reg1 = aXX_readDec(elems[1] + 1);
            *fullCode = DSP_R; //We set the opperand
            *fullCode |= (reg1 & REG_MASK ) << ARG_SHIFT_1 ; //We set the register
            return COMPILED_LINE_INSTRUCTION;
        }else{
            fprintf(stderr,"    Wrong argument for DSP operation.\n");
            return COMPILED_LINE_NOT_OK;
        }
    }else{
        fprintf(stderr,"    Unknown operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    fprintf(stderr,"    Unreadable line.\n");
    return COMPILED_LINE_NOT_OK;
}

