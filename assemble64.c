/*----------------------------------------------------------------\
|This file contant what is needed to precompile assembly code into|
|a object file for 64 bits machine code.                          |
\----------------------------------------------------------------*/

#include "assemble64.h"

/*
 * This function take a file containing assembly code for the asvm
 * and compile it insto an asvm object file
 *  Arguments :
 *      fin : pointer to the assembly code file
 *      fout : pointer to the object file
 *  return :
 *      COMPILATION_OK if everything went well
 *      COMPILATION_ERROR otherwise
 */
uint8_t a64_assemble(FILE* fin,FILE* fout){
    OBJECT_MW_TYPE magicWord = OBJECT_MW; //Writing the magic word
    fwrite(&magicWord, 1, OBJECT_MW_SIZE , fout);
    char* line = malloc(SIZELINE);
    uint64_t lineNumber = 1;
    char** elems = malloc(sizeof(char*) * 150); //We will never get 150 words
    fgets(line,SIZELINE,fin);
    while(!feof(fin)){
        uint8_t n = aXX_preprocessLine(line,elems);
        if(n) //no point in compiling if there is no elements
        if(strlen(elems[0]) > 0){ //no point in compiling if there is no opperand
            uint8_t a = a64_compileLine(elems,n,line);
            if(a == COMPILED_LINE_INSTRUCTION){
                fprintf(fout,"i");
                fwrite(line,8,1,fout);
            }else if(a == COMPILED_LINE_BRANCH){
                fprintf(fout,"j");
                fwrite(line, SIZELINE_LABEL,1,fout);
            }else if(a == COMPILED_LINE_DATA){
                fprintf(fout,"d");
                fwrite(line, SIZELINE, 1, fout);
            }else{
                fprintf(stderr,"Error line %" PRIx64 ":\n",lineNumber);
                return COMPILATION_ERROR;
            }
        }
        aXX_freeElems(n,elems);
        fgets(line,SIZELINE,fin);
        lineNumber++;
    } 
    return COMPILATION_OK;
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
    memset(ret, 0, SIZELINE);
    uint64_t* fullCode = (uint64_t*) ret; //representing the output as a 64 bit number
    if(!strcmp(elems[0],"NIL")){
        a64_createMachineCode(NIL, NULL, 1, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else if(!strcmp(elems[0],"QUIT")){
        uint8_t config = a64_analyzeLine(elems, n);
        if(config == COMPILE_NOPE){
            a64_createMachineCode(QUIT, NULL, 1, fullCode);
        }else if(config == COMPILE_N){
            a64_createMachineCode(QUIT, elems, 2, fullCode);
        }else{
            fprintf(stderr, "    Worng arguments for QUIT opperation.\n");
            return COMPILED_LINE_NOT_OK;
        }
        return COMPILED_LINE_INSTRUCTION;
    }else if(!strcmp(elems[0],"MOV") && n == 3){
        uint8_t config = a64_analyzeLine(elems, n);
        if(config == COMPILE_RR){
            a64_createMachineCode(MOV_RR, elems, 3, fullCode);
            return COMPILED_LINE_INSTRUCTION;
        }else if(config == COMPILE_RN){
            a64_createMachineCode(MOV_RN, elems, 3, fullCode);
            return COMPILED_LINE_INSTRUCTION;            
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
    }else if(!strcmp(elems[0],"AND")){
        return a64m_and(elems, n, fullCode);
    }else if(!strcmp(elems[0],"OR")){
        return a64m_or(elems, n, fullCode);
    }else if(!strcmp(elems[0],"XOR")){
        return a64m_xor(elems, n, fullCode);
    }else if(!strcmp(elems[0],"NOT")){
        return a64m_not(elems, n, fullCode);
    }else if(!strcmp(elems[0],"PCHAR")){
        uint8_t config = a64_analyzeLine(elems, n);
        if(config == COMPILE_R){
            a64_createMachineCode(PCHAR, elems, 2, fullCode);
            return COMPILED_LINE_INSTRUCTION;
        }else{
            fprintf(stderr,"    Wrong argument for DSP operation.\n");
            return COMPILED_LINE_NOT_OK;
        }
    }else if(!strcmp(elems[0],"DSP")){
        uint8_t config = a64_analyzeLine(elems, n);
        if(config == COMPILE_R){
            a64_createMachineCode(DSP_R, elems, 2, fullCode);
            return COMPILED_LINE_INSTRUCTION;
        }else{
            fprintf(stderr,"    Wrong argument for DSP operation.\n");
            return COMPILED_LINE_NOT_OK;
        }
    }else if(!strcmp(elems[0],"PUSH")){
        uint8_t config = a64_analyzeLine(elems, n);
        if(config == COMPILE_R){
            a64_createMachineCode(PUSH, elems, 2, fullCode);
            return COMPILED_LINE_INSTRUCTION;
        }else{
            fprintf(stderr,"    Wrong argument for PUSH operation.\n");
            return COMPILED_LINE_NOT_OK;
        }
    }else if(!strcmp(elems[0],"POP")){
        uint8_t config = a64_analyzeLine(elems, n);
        if(config == COMPILE_R){
            a64_createMachineCode(POP, elems, 2, fullCode);
            return COMPILED_LINE_INSTRUCTION;
        }else{
            fprintf(stderr,"    Wrong argument for POP operation.\n");
            return COMPILED_LINE_NOT_OK;
        }
    }else if(!strcmp(elems[0],"LAB") || !strcmp(elems[0],"FUNC")){
        return a64j_lab( elems, n, ret);
    }else if(!strcmp(elems[0],"JMP")){
        return a64j_jmp(elems, n, ret);
    }else if(!strcmp(elems[0],"JZ")){
        return a64j_jz(elems, n, ret);
    }else if(!strcmp(elems[0],"JNZ")){
        return a64j_jnz(elems, n, ret);
    }else if(!strcmp(elems[0],"JB")){
        return a64j_jb(elems, n, ret);
    }else if(!strcmp(elems[0],"JBE")){
        return a64j_jbe(elems, n, ret);
    }else if(!strcmp(elems[0],"JS")){
        return a64j_js(elems, n, ret);
    }else if(!strcmp(elems[0],"JSE")){
        return a64j_jse(elems, n, ret);
    }else if(!strcmp(elems[0],"CMP")){
        return a64j_cmp(elems, n, fullCode);
    }else if(!strcmp(elems[0],"CALL")){
        return a64j_call(elems, n, ret);
    }else if(!strcmp(elems[0],"RET")){
        return a64j_ret(elems, n, fullCode);
    }else if(!strcmp(elems[0],"FREE")){
        return a64m_free(elems, n, fullCode);
    }else if(!strcmp(elems[0],"ASK")){
        return a64m_ask(elems, n, fullCode);
    }else if(!strcmp(elems[0],"ASK_BYTE")){
        return a64m_ask_byte(elems, n, fullCode);
    }else if(!strcmp(elems[0],"STR")){
        return a64m_str(elems, n, fullCode);
    }else if(!strcmp(elems[0],"LDR")){
        return a64m_ldr(elems, n, fullCode);
    }else if(!strcmp(elems[0],"STR_BYTE")){
        return a64m_str_byte(elems, n, fullCode);
    }else if(!strcmp(elems[0],"LDR_BYTE")){
        return a64m_ldr_byte(elems, n, fullCode);
    }else if(!strcmp(elems[0],"DATA")){
        return a64m_data(elems, n ,ret);
    }else if(!strcmp(elems[0],"LDR_DATA")){
        return a64m_ldr_data(elems, n, ret);
    }else{
        fprintf(stderr,"    Unknown operation : %s.\n",elems[0]);
        return COMPILED_LINE_NOT_OK;
    }
    fprintf(stderr,"    Unreadable line.\n");
    return COMPILED_LINE_NOT_OK;
}

/*
 * Read the line of code and says how what kind of arguments we have
 *  Arguments :
 *      elems : a pointer to the elements of the line of code
 *      n : the numbers of element
 *  return : 
 *      COMPILE_NOPE : if there is no arguments to the operation
 *      COMPILE_R : if there is a single register as argument
 *      COMPILE_N : if there is a single number as argument
 *      COMPILE_RR : if there is two registers as argument
 *      COMPILE_RRR : if there is three registers as argument
 *      COMPILE_RN : if there is a register followed by a number an argument
 *      COMPILED_LINE_NOT_OK if there is an error
 */
uint8_t a64_analyzeLine(char** elems, uint8_t n){
    if(n == 1){
        return COMPILE_NOPE;
    }else if(n == 2 && elems[1][0] == 'R'){
        return COMPILE_R;
    }else if(n == 2){
        return COMPILE_N;
    }else if(n == 3 && elems[1][0] == 'R' && elems[2][0] == 'R'){
        return COMPILE_RR;
    }else if(n == 3 && elems[1][0] == 'R'){
        return COMPILE_RN;
    }else if(n == 4 && elems[1][0] == 'R' && elems[2][0] == 'R' && elems[3][0] == 'R'){
        return COMPILE_RRR;
    }else{
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * Create a machine code from an opperand and the argument
 *  Arguments :
 *      opperand : the opperand of the line of machine code
 *      elems : the arguments of the operation
 *      n : the number of arguments
 *      fullCode : a pointer to the position of the machinecode
 *  return :
 *      the same thing as a64_analyzeLine
 */
uint8_t a64_createMachineCode(uint64_t opperand, char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config){
        uint8_t reg1, reg2, reg3;
        uint64_t num1;
        switch(config){
            case COMPILE_NOPE :
                *fullCode = opperand;
                break;
            case COMPILE_R :
                reg1 = aXX_readDec(elems[1] + 1);
                *fullCode = opperand;
                *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
                break;
            case COMPILE_N :
                *fullCode = opperand;
                if(elems[1][0] == 'X')
                    num1 = aXX_readHex(elems[1] + 1);
                else
                    num1 = aXX_readDec(elems[2]);
                *fullCode |= (num1 & NUM0_MASK) << ARG_SHIFT_1;
                break;
            case COMPILE_RR :
                reg1 = aXX_readDec(elems[1] + 1);
                reg2 = aXX_readDec(elems[2] + 1);
                *fullCode = opperand;
                *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
                *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
                break;
            case COMPILE_RRR :
                reg1 = aXX_readDec(elems[1] + 1);
                reg2 = aXX_readDec(elems[2] + 1);
                reg3 = aXX_readDec(elems[3] + 1);
                *fullCode = opperand;
                *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
                *fullCode |= (reg2 & REG_MASK) << ARG_SHIFT_2;
                *fullCode |= (reg3 & REG_MASK) << ARG_SHIFT_3;
                break;
            case COMPILE_RN :
                reg1 = aXX_readDec(elems[1] + 1);
                *fullCode = opperand;
                *fullCode |= (reg1 & REG_MASK) << ARG_SHIFT_1;
                if(elems[2][0] == 'X')
                    num1 = aXX_readHex(elems[2] + 1);
                else
                    num1 = aXX_readDec(elems[2]);
                *fullCode |= (num1 & NUM1_MASK) << ARG_SHIFT_2;
                break;
            default :
                return COMPILED_LINE_NOT_OK;
        }
        }else{
            return COMPILED_LINE_NOT_OK;
        }
    return config;
}

