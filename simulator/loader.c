/*-------------------------------------------------------------\
|This file is used to load files into the RAM of the simulator.|
\-------------------------------------------------------------*/

#include "loader.h"
static bool magic_word_ok(asrm* vm);

/*
 * Create a asrm simulator and load a file in its RAM
 *  Argument:
 *      filename : the file with the content to be put in RAM
 *  return:
 *      A pointer to the asrm struct if everything is OK
 *      A NULL pointer otherwise
 */
asrm* load_file(const char* filename){
    asrm* ret = asrm_init();
    //opening file
    FILE* f;
    if((f = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Error, unable to read file %s.\n", filename);
        asrm_free(ret);
        return NULL;
    } 
    //reading file
    char* ram_char = (char*) ret->ram;
    for(int i=0; i<RAM_SIZE_BYTE; i++){
        char ch = fgetc(f);
        if(feof(f))
            break;
        ram_char[i] = ch;
    }
    fclose(f);
    //cheaking for errors
    if(MAGIC_WORD_CHECKING){
        bool no_error = magic_word_ok(ret);
        if(MAGIC_WORD_CHECKING == MAGIC_WORD_WARNING && !no_error)
            fprintf(stderr, "Warning: magic word not found.\n");
        if(MAGIC_WORD_CHECKING == MAGIC_WORD_ERROR && !no_error){
            fprintf(stderr, "Error: magic word not founr.\n");
            asrm_free(ret);
            return NULL;
        }
    }
    return ret;
}

/*
 * Check if the magic word is found at the beggining of the loaded RAM
 */
static bool magic_word_ok(asrm* vm){
    const char* mw = MAGIC_WORD;
    char* ram_char = (char*)  vm->ram;
    for(int i=0; i<MAGIC_WORD_SIZE; i++)
        if(mw[i] != ram_char[i])
            return false;
    return true;
}

