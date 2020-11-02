#include "pico_assembler.h"

enum processLine_return{OK, NO_CODE, ERROR};
static enum processLine_return processLine(const char* line, uint8_t* ret);
static enum processLine_return preprocessLine(const char* line, char** ret);
static bool makeInst(uint8_t opperand, bool isRegister, char* arg, uint8_t* ret);
static void toLowerCase(char* str);

void mini_assembleFile(const char* fileIn, const char* fileOut){
    //opening files
    FILE* in;
    FILE* out;
    if((in = fopen(fileIn, "r")) == NULL){
        fprintf(stderr, "Error : unable to open file %s.\n", fileIn);
        exit(RET_UNOPEN_FILE);
    }
    if((out = fopen(fileOut, "w")) == NULL){
        fprintf(stderr, "Error : unable to open file %s.\n", fileOut);
        exit(RET_UNOPEN_FILE);
    }
    //writing magic word
    fputc('A', out);
    fputc('S', out);
    fputc('R', out);
    fputc('M', out);
    //assembling
    char* line = malloc(LINE_BUFF);
    int lineNumber = 1;
    bool noErrors = true;
    while(fgets(line, LINE_BUFF, in) != NULL){
        uint8_t inst;
        enum processLine_return pross = processLine(line, &inst);
        switch(pross){
            case OK:
                fputc(inst, out);
                break;
            case ERROR:
                fprintf(stdout, "Error line %i.\n", lineNumber);
                noErrors = false;
                break;
            case NO_CODE:
                //Nothing to do here
                break;
        }
        lineNumber++;
    }
    if(!noErrors)
        puts("error behavious TODO");
    //cleaning
    free(line);
    fclose(in);
    fclose(out);
}

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
    bool instOK = true;
    toLowerCase(toPrePross[0]);
    if(preProssRes == OK){
        if(mnSLP(toPrePross[0])){
            *ret = SLP;
        }else if(mnSET(toPrePross[0])){
            instOK = makeInst(SET, false, toPrePross[1], ret);
        }else if(mnREAD(toPrePross[0])){
            instOK = makeInst(READ, true, toPrePross[1], ret);
        }else if(mnCPY(toPrePross[0])){
            instOK = makeInst(CPY, true, toPrePross[1], ret);
        }else if(mnADD(toPrePross[0])){
            instOK = makeInst(ADD, true, toPrePross[1], ret);
        }else if(mnSUB(toPrePross[0])){
            instOK = makeInst(SUB, true, toPrePross[1], ret);
        }else if(mnAND(toPrePross[0])){
            instOK = makeInst(AND, true, toPrePross[1], ret);
        }else if(mnOR(toPrePross[0])){
            instOK = makeInst(OR, true, toPrePross[1], ret);
        }else if(mnXOR(toPrePross[0])){
            instOK = makeInst(XOR, true, toPrePross[1], ret);
        }else if(mnNOT(toPrePross[0])){
            instOK = makeInst(NOT, true, toPrePross[1], ret);
        }else if(mnLSL(toPrePross[0])){
            instOK = makeInst(LSL, true, toPrePross[1], ret);
        }else if(mnLSR(toPrePross[0])){
            instOK = makeInst(LSR, true, toPrePross[1], ret);
        }else if(mnEQ(toPrePross[0])){
            instOK = makeInst(EQ, true, toPrePross[1], ret);
        }else if(mnLES(toPrePross[0])){
            instOK = makeInst(LES, true, toPrePross[1], ret);
        }else if(mnSTR(toPrePross[0])){
            instOK = makeInst(STR, true, toPrePross[1], ret);
        }else if(mnLOAD(toPrePross[0])){
            instOK = makeInst(LOAD, true, toPrePross[1], ret);
        }else if(mnJMP(toPrePross[0])){
            *ret = JMP;
        }else if(mnJIF(toPrePross[0])){
            *ret = JIF;
        }else if(mnPOP(toPrePross[0])){
            *ret = POP;
        }else if(mnPUSH(toPrePross[0])){
            *ret = PUSH;
        }else if(mnCALL(toPrePross[0])){
            *ret = CALL;
        }else if(mnRET(toPrePross[0])){
            *ret = RET;
        }else if(mnQUIT(toPrePross[0])){
            *ret = QUIT;
        }else if(macroRAWBYTE(toPrePross[0])){
            int byte = atoi(toPrePross[1]);
            if( (uint8_t) byte != byte){
                fprintf(stderr, "Error, value after rawbyte is more than a byte.\n");
                preProssRes = ERROR;
            }
            *ret = (uint8_t) byte;
        }else{
            fprintf(stderr, "Error, unknown instruction.\n");
            preProssRes = ERROR; //instruction undetermined
        }
    }
    if(!instOK){
        fprintf(stderr, "Error, unable to read a line, invalid format.\n");
        preProssRes = ERROR;
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
                if(line[i] == ';' || line[i] == '\n'){
                    return NO_CODE;
                }else if(line[i] == ' ' || line[i] == '	'){
                    //do nothing, wait for a word
                }else{
                    ret[0][0] = line[i];
                    now = 1;
                }
                break;
            case 1: //fist word
                if(line[i] == ';' || line[i] == '\n')
                    return OK;
                else if(line[i] == ' ' || line[i] == '	')
                    now = 2;
                else
                    ret[0][strlen(ret[0])] = line[i];
                break;
            case 2: //before the second word
                if(line[i] == ';' || line[i] == '\n'){
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

/*
 * Create an instruction given an opperand and an argument
 *  Arguments:
 *      opperand : the opperand of the instruction
 *      isResgister : is the argument a register or a simple number
 *      arg : the instruction's argument
 *      ret : a pointer to where the instruction will be written
 *  return:
 *      true if the form of the argument is correct, false oterwise
 */
static bool makeInst(uint8_t opperand, bool isRegister, char* arg, uint8_t* ret){
    bool isNamedRegister = !(strcmp(arg, "WR") && strcmp(arg, "SR") && strcmp(arg, "SP") && strcmp(arg, "PC")); //Is the register a special register
    if(isRegister && (arg[0] != 'r' && arg[0] != 'R' && !isNamedRegister)){ //A register must be notted with a starting R or with its name
        fprintf(stderr, "Error, no register while one was expected\n");
        return false;
    }
    if(isNamedRegister && !isRegister){ //No point in using a name if this is not a register
        fprintf(stderr, "Error, using a named register while a number was expeted.\n");
        return false;
    }
    uint32_t reg;
    if(isNamedRegister){
        if(!strcmp(arg, "WR"))
            reg = 0;
        if(!strcmp(arg, "SR"))
            reg = 13;
        if(!strcmp(arg, "PC"))
            reg = 14;
        if(!strcmp(arg, "SP"))
            reg = 15;
    }else{
        int offset = (isRegister ? 1 : 0); //If it is a register there is a 'r' at the begining
        int numRead = sscanf(arg+offset, "%u", &reg);
        if(numRead != 1){
            fprintf(stderr, "Error, unable to read number\n");
            return false;
        }
        if(reg > 15){
            fprintf(stderr, "Error, using a register above 15.\n");   
            return false;
        }
    }
    *ret = ((opperand & 0xF) << 4) | (reg & 0xF);
    return true;
}

static void toLowerCase(char* str){
    for(size_t i=0; i<strlen(str); i++)
        if( 65 <= str[i] && str[i] <= 90)
            str[i] += 32;
}

