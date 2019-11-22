#include "assemble64.h"

void a64_assemble(FILE* fin,FILE* fout){
    char* line = malloc(255);
    char** elems = malloc(sizeof(char*) * 150); //We will never get 150 words
    fgets(line,255,fin);
    while(line != NULL && strlen(line) > 2){
        uint8_t n = aXX_preprocessLine(line,elems);
        uint8_t a = a64_compileLine(elems,n,line);
        if(a == 1){
            fprintf(fout,"i");
            fwrite(line,8,1,fout);
        }else if(a == 2){
            fprintf(fout,"j");
            fwrite(line,200,1,fout);
        }else{
            return;
        }
        
        aXX_freeElems(n,elems);
        fgets(line,255,fin);
    } 
}

/*
 * compile a line of assembly language
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      ret : the compiled code
 * Return :
 *      1 if the result is a compiled instruction
 *      2 if the result is about branching
 *      0 if there is an error
 */
uint8_t a64_compileLine(char** elems,uint8_t n,char* ret){
    memset(ret,0,strlen(ret));
    uint64_t* fullCode = (uint64_t*) ret; //representing the output as a 64 bit number
    if(!strcmp(elems[0],"MOV") && n == 3){
        if(elems[1][0] == 'R'){
            if(elems[2][0] == 'R'){ //A register
                //todo
            }else if(elems[2][0] == 'X'){ //An hexadecimal number
                uint8_t reg = aXX_readDec(elems[1] + 1);
                uint64_t num = aXX_readHex(elems[2] + 1);
                *fullCode = MOV_RN; //We set the opperand
                *fullCode |= (reg & REG_MASK ) << 10; //We set the register
                *fullCode |= (num & NUM1_MASK ) << 16; //We set the data
                return 1;
            }else{ //A decimal number
                uint8_t reg = aXX_readDec(elems[1] + 1);
                uint64_t num = aXX_readDec(elems[2]);
                *fullCode = MOV_RN; //We set the opperand
                *fullCode |= (reg & REG_MASK ) << 10; //We set the register
                *fullCode |= (num & NUM1_MASK ) << 16; //We set the data
                return 1;
            }
        }else{
            return 0;
        }
    }else if(!strcmp(elems[0],"DSP") && n == 2){
        if(elems[1][0] == 'R'){
            uint8_t reg = aXX_readDec(elems[1] + 1);
            *fullCode = DSP_R; //We set the opperand
            *fullCode |= (reg & REG_MASK ) << 10; //We set the register
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
    return 0;
}


