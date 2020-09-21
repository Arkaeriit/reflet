#include "mimi_assembler.h"

enum processLine_return{OK, NO_CODE, ERROR};
static enum processLine_return processLine(const char* line, uint8_t* ret);
static enum processLine_return preprocessLine(const char* line, char** ret);

/*
 * Assemble a line of assembly language into a byte of machine code
 *  Arguments:
 *      line : the line of assembly language
 *      ret : a pointer to where the machine code will be put
 *  return:
 *      OK if a byte of machine code have been created
 *      NO_CODE if there was no assembly code on the line
 *      ERROR if there was invalid assembly code
 */
static enum processLine_return processLine(const char* line, uint8_t* ret){
    char** toPrePross = malloc(sizeof(char*) * 2);
    toPrePross[0] = malloc(100);
    toPrePross[1] = malloc(100);
    enum processLine_return preProssRes = preprocessLine(line, toPrePross);
    if(preProssRes == OK){
        if(mnSLP(toPrePross[0])){
            *ret = SLP;
        }else if(mnSET(toPrePross[0])){}

    
    }
    free(toPrePross[0]);
    free(toPrePross[1]);
    free(toPrePross);
    return preProssRes;
}

/*
 * Take a line and separate cleanely the mnemonic and the argument.
 *  Arguments:
 *      line : the line of assembly language
 *      ret : a pointer to at least 2 char* of sufisant size
 *  return:
 *      OK if a byte of machine code have been created
 *      NO_CODE if there was no assembly code on the line
 *      ERROR if there was invalid assembly code
 */
static enum processLine_return preprocessLine(const char* line, char** ret){
    memset(ret[0], 0, 100);
    memset(ret[1], 0, 100);
    int now = 0;
    for(size_t i=0; i<strlen(line); i++){
        switch(now){
            case 0: //before the first word
                if(line[i] == ';'){
                    return NO_CODE;
                }else if(line[i] == ' ' || line[i] == '	'){
                    //do nothing, wait for a word
                }else{
                    ret[0][0] = line[i];
                    now = 1;
                }
                break;
            case 1: //fist word
                if(line[i] == ';')
                    return OK;
                else if(line[i] == ' ' || line[i] == '	')
                    now = 2;
                else
                    ret[0][strlen(ret[0])] = line[i];
                break;
            case 2: //before the second word
                if(line[i] == ';'){
                    return OK;
                }else if(line[i] == ' ' || line[i] == '	'){
                    //do nothing, wait for a word
                }else{
                    ret[1][0] = line[i];
                    now = 3;
                }
                break;
            case 3: //second word
                if(line[i] == ';' || line[i] == '\n')
                    return OK;
                else if(line[i] == ' ' || line[i] == '	' || line[i] == ';' || line[i] == '\n')
                    now = 4;
                else
                    ret[1][strlen(ret[1])] = line[i];
                break;
            default:
                if(line[i] == ';' || line[i] == '\n')
                    return OK;
                else if(!(line[i] == ' ' || line[i] == '	' || line[i] == ';' || line[i] == '\n'))
                    return ERROR;
        }
    }
    return ERROR;
}

