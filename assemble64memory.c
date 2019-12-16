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

/*
 * compile a line of assembly language coding for a str opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_str(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(STR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for ASK_BYTE operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for a ldr opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_ldr(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(LDR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for ASK_BYTE operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for a str_byte opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_str_byte(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(STR_BYTE, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for ASK_BYTE operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * compile a line of assembly language coding for a ldr_byte opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation of the line went well
 *      COMPILED_LINE_INSTRUCTION if there is an error
 */
uint8_t a64m_ldr_byte(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(LDR_BYTE, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }else{
        fprintf(stderr,"    Wrong argument for ASK_BYTE operation.\n"); 
        return COMPILED_LINE_NOT_OK;
    }
}

/*
 * Let us read a line of data from an assembly.
 *  Aruguments :
 *      elems : the arguments of the line of assembly
 *      n : the size of elems
 *      ret : where we will put the precompiled result
 *  return :
 *      COMPILED_LINE_DATA if everything went well
 */
uint8_t a64m_data(char** elems, uint8_t n, char* ret){
    bool* isString = malloc(sizeof(bool) * n);
    n = a64m_concatStrings(elems, n, isString);
    *ret = 'd';
    *((uint16_t*) (ret+1)) = DATA;
    strcpy(ret + 3,elems[1]);
    char* pointerString = ret + SIZELINE_LABEL;
    for(uint8_t i=2; i<n; i++){ //We start at 2 because the first element is the opperand and the second is the label name
        if(isString[i]){
            memcpy(pointerString,elems[i],strlen(elems[i])); //We add the string to the rest of the data
            pointerString += strlen(elems[i]);
            elems[i] = elems[i] - 1; //Since we cut the " from the string we have to get it back in order to free the pointer
        }else{
            uint8_t num;
            if(elems[i][0] == 'X')
                num = aXX_readHex(elems[i] + 1);
            else
                num = aXX_readDec(elems[i]);
            *pointerString = num;
            pointerString ++;
        }
    }
    return COMPILED_LINE_DATA;
}
 
/*
 * When we want to set data we want to concatenate string entered as input.
 *  Arguments :
 *      elems : the arguments of the line of assembly
 *      n : the size of elems
 *      isString : an array of boolean stating if each 
 *                 elements of the new elems is a string
 *  return :
 *      the size of elem after all concatenation
 */
uint8_t a64m_concatStrings(char** elems, uint8_t n,bool* isString){
    char* space = " ";
    char** elemsRet = malloc(sizeof(char*) * 100); //overkill amount of args
    uint8_t ret = 2; //We start at 1 because we don't touch the opperand nor the name of the data label
    elemsRet[0] = elems[0];
    elemsRet[1] = elems[1];
    bool flagString = false; //indicate if we entered a string
    int startString; //indicate the start of the current string
    for(int i=2; i<n; i++){
        if(!flagString){
            isString[ret] = false; //we assume that the element is not a string
            if(elems[i][0] == '"'){
                flagString = true;
                elems[i] = elems[i] + 1; //We dont want the " in the final product
                startString = i;
                isString[ret] = true; // if it is a string we correct ourselves
            }
            elemsRet[ret] = elems[i];
            ret++; //each time we have a new argument while not on the string ther is something new in the returnd elements
        }
        if(flagString){
            if(i != startString){ //if we are not in the same elems as the start of the line we concatenate the arguments
                strcat(elemsRet[ret-1],space);
                strcat(elemsRet[ret-1],elems[i]);
            }
            char* stringToComplete = elemsRet[ret-1];
            if(stringToComplete[strlen(stringToComplete) - 1] == '"'){
                elemsRet[ret - 1][strlen(elemsRet[ret-1]) - 1] = 0; //We dont want the " in the final product
                flagString = false;
            }
        }
    }
    for(int i=0; i<ret; i++){ //at the end we put the new array in the old one
        elems[i] = elemsRet[i];
    }
    free(elemsRet);
    return ret;
}

/*
 * compile a line of assembly language coding for a LDR_DATA opperation
 * Arguments:
 *      elems : the result of aXX_preprocessLine
 *      n : the size of elems
 *      fullCode : the compiled instruction
 * Return :
 *      COMPILED_LINE_NOT_OK if the compilation had an error
 *      COMPILED_LINE_DATA if there is no error
 */
uint8_t a64m_ldr_data(char** elems, uint8_t n, char* ret){
    if(n != 3){
        fprintf(stderr,"    Wrong argument for LDR_DATA operation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    *ret = 'l';
    *((uint16_t*) (ret+1)) = LDR_DATA; //We define the opperand
    strcpy(ret + 3,elems[1]); //We add the name of the label 3 bytes into the instruction
    uint8_t reg;
    if(elems[2][0] == 'R'){
        reg = aXX_readDec(elems[2] + 1);
    }else{
        fprintf(stderr,"    Wrong argument for LLDR_DATA opperation.\n");
        return COMPILED_LINE_NOT_OK;
    }
    memcpy(ret + SIZELINE_LABEL, &reg, 1);
    return COMPILED_LINE_DATA;
}

