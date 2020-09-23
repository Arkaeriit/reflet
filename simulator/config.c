#include "config.h"

enum processLine_return{OK, NO_LINE, ERROR};
static enum processLine_return preprocessLine(const char* line, char** ret);

/*
 * Reads a config file and apply the configuration to the simulator
 *  Arguments:
 *      vm : the simulator
 *      configFile : path to the config file
 *  errors:
 *      * If an error appends, the program stops with the exit code
 *      RET_CONFIG
 *      * If the config file can't be read, the program stops with the
 *      exit code RET_UNOPEN_FILE
 */
void applyConfig(asrm* vm, const char* configFile){
    //opening file
    FILE* f;
    if((f = fopen(configFile, "r")) == NULL){
        fprintf(stderr, "Error : unable to open file %s.\n", configFile);
        exit(RET_UNOPEN_FILE);
    }
    char* line = malloc(LINE_BUFF);
    int lineNumber = 1;
    char** resultBuff = malloc(sizeof(char*) * 2);
    resultBuff[0] = malloc(100);
    resultBuff[1] = malloc(100);
    //Reading each line of config
    while(fgets(line, LINE_BUFF, f) != NULL){
        enum processLine_return pross = preprocessLine(line, resultBuff);
        switch(pross){
            case OK:
                {
                    //Checking if we can use the config value
                    word_t configValue;
                    int numRead = sscanf(resultBuff[1], "%lu", &configValue);
                    if(numRead != 1){
                        fprintf(stdout, "Error line %i.\n", lineNumber);
                        exit(RET_CONFIG);
                    }
                    //Appliyng the config line
                    if(!strcmp(resultBuff[0], "word_size")){
                        if(configValue%8){
                            fprintf(stdout, "Error line %i.\n", lineNumber);
                            exit(RET_CONFIG);
                        }
                        vm->config->word_size = configValue;
                    }else if(!strcmp(resultBuff[0], "ram_size")){
                        vm->config->ram_size = configValue;
                    }else{
                        fprintf(stdout, "Error line %i.\n", lineNumber);
                        exit(RET_CONFIG);
                    }
                }
                break;
            case ERROR:
                fprintf(stdout, "Error line %i.\n", lineNumber);
                exit(RET_CONFIG);
                break;
            case NO_LINE:
                //Nothing to do here
                break;
        }
        lineNumber++;
    }
    //computing implicit parameters
    vm->config->word_size_byte = vm->config->word_size/8;
    word_t mask = 0;
    for(int i=0; i<vm->config->word_size_byte; i++){
        mask = mask << 8;
        mask = mask + 0xFF;
    }
}

/*
 * Take a line and separate cleanely the parameter and the value.
 *  Arguments:
 *      line : the line of configuration
 *      ret : a pointer to at least 2 char* of sufisant size
 *  return:
 *      OK if there is a parameter and a value
 *      NO_LINE if there was no configuration
 *      ERROR if the line is not valid
 */
static enum processLine_return preprocessLine(const char* line, char** ret){
    memset(ret[0], 0, 100);
    memset(ret[1], 0, 100);
    int now = 0;
    for(size_t i=0; i<strlen(line); i++){
        switch(now){
            case 0: //before the first word
                if(line[i] == ';' || line[i] == '\n'){
                    return NO_LINE;
                }else if(line[i] == ' ' || line[i] == '	'){
                    //do nothing, wait for a word
                }else{
                    ret[0][0] = line[i];
                    now = 1;
                }
                break;
            case 1: //fist word
                if(line[i] == ';' || line[i] == '\n')
                    return ERROR; //there has to be two words
                else if(line[i] == ' ' || line[i] == '	')
                    now = 2;
                else
                    ret[0][strlen(ret[0])] = line[i];
                break;
            case 2: //before the second word
                if(line[i] == ';' || line[i] == '\n'){
                    return ERROR; //there has to be two words
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

