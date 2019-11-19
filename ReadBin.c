#include "ReadBin.h"

/*
 * Get a file, return if the file is readable, if it is a 64-bit binairy and it's size.
*/
infofile rb_analyze(const char* filename){
    infofile ret;
    struct stat st; 
    if (stat(filename, &st) == 0){
        ret.fileSize = st.st_size;
    }else{
        ret.flagCorrect = 0;
        return ret;
    }
    FILE* fp = fopen(filename,"r");
    uint16_t bufferReadStart; //buffer to read the 2-byte magic word
    fread(&bufferReadStart,1,2,fp);
    if(bufferReadStart == 0x64AC){
        ret.flag64 = 1;
        if( (ret.fileSize-2)%8 == 0) //instructions are 64 bytes
            ret.flagCorrect = 1;
        else
            ret.flagCorrect = 0;
    }else if(bufferReadStart == 0x32AC){
        ret.flag64 = 0;
        ret.flag64 = 1;
        if( (ret.fileSize-2)%4 == 0) //instructions are 32 bytes
            ret.flagCorrect = 1;
        else
            ret.flagCorrect = 0;
    }else{
        ret.flagCorrect = 0;
    }
    return ret;
}

/*
 * Read the content of filename ant init a vm with it.
 * Arguments:
 *      vm : the pointer to the vm we want to initialize
 *      filename : the name of the file whre the machinecode for the VM is stored
 *      fileSize : size of the file
 *  return :
 *      0 if everything if 0
 *      1 if the file can't be read
 */
int rb_init64(vm_64* vm, const char* filename, uint32_t fileSize){
    FILE* fp = fopen(filename,"r");
    uint16_t a; //useless value
    fread(&a,2,1,fp); //the 2 first bits are ignored
    vm->flags = 0;
    vm->nombreInstruction = (fileSize-2) / 8;
    vm->code = malloc(sizeof(uint64_t) * vm->nombreInstruction);
    if(fread(vm->code,8,vm->nombreInstruction,fp) == vm->nombreInstruction)
        return 0;
    return 1;
}

