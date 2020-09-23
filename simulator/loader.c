/*-------------------------------------------------------------\
|This file is used to load files into the RAM of the simulator.|
\-------------------------------------------------------------*/

#include "loader.h"
static bool magic_word_ok(asrm* vm);

/*
 * Create a asrm simulator and load a file in its RAM
 *  Argument:
 *      filename : the file with the content to be put in RAM
 *      vm : a pointer to an initialised but not loaded asrm struct
 *  return:
 *      true if the loading went right
 *      false in case of an issue
 */
bool load_file(const char* filename, asrm* vm){
    if(vm->ram != NULL)
        free(vm->ram);
    asrm_initRAM(vm);
    //opening file
    FILE* f;
    if((f = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Error, unable to read file %s.\n", filename);
        asrm_free(vm);
        return false;
    } 
    //reading file
    char* ram_char = (char*) vm->ram;
    for(int i=0; i<vm->config->ram_size; i++){
        char ch = fgetc(f);
        if(feof(f))
            break;
        ram_char[i] = ch;
    }
    fclose(f);
    //cheaking for errors
    if(MAGIC_WORD_CHECKING){
        bool no_error = magic_word_ok(vm);
        if(MAGIC_WORD_CHECKING == MAGIC_WORD_WARNING && !no_error)
            fprintf(stderr, "Warning: magic word not found.\n");
        if(MAGIC_WORD_CHECKING == MAGIC_WORD_ERROR && !no_error){
            fprintf(stderr, "Error: magic word not founr.\n");
            asrm_free(vm);
            return false;
        }
    }
    return true;
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

