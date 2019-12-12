/*--------------------------------------------\
|This file contain what is needed to check    |
|if a binary file contain usable asvm machine |
|code and if it do so it loads it into a vm.  |
\--------------------------------------------*/

#include "ReadBin.h"

/*
 * Get a file, return if the file is readable, if it is a 64-bit binairy and it's size.
*/
infofile rb_analyze(const char* filename){
    infofile ret;
    struct stat st; //Getting the file size
    if (stat(filename, &st) == 0){
        ret.fileSize = st.st_size;
    }else{
        ret.flagCorrect = false;
        return ret;
    }
    FILE* fp = fopen(filename,"r");
    BINARY_MW_TYPE bufferReadStart = 0; //buffer to read the magic word
    fread(&bufferReadStart,1, BINARY_MW_SIZE ,fp);
    fclose(fp);
    if(bufferReadStart == BINARY_MW ){
        ret.flag64 = true;
        if( (ret.fileSize- BINARY_MW_SIZE  )%8 == 0) //instructions are 64 bytes
            ret.flagCorrect = true;
        else
            ret.flagCorrect = false;
    }else if(bufferReadStart == BINARY_32_MW ){
        ret.flag64 = false;
        if( (ret.fileSize- BINARY_MW_SIZE )%4 == 0) //instructions are 32 bytes
            ret.flagCorrect = true;
        else
            ret.flagCorrect = false;
    }else{
        ret.flagCorrect = false;
    }
    return ret;
}

/*
 * Read the content of filename ant init a vm with it.
 * Arguments:
 *      vm : the pointer to the vm we want to initialize
 *      filename : the name of the file whre the machinecode for the VM is stored
 *      fileSize : size of the file
 *      vmArgs : the arguments passed to the vm
 *  return :
 *      READ_OK if everything if OK
 *      READ_NOT_OK if the file can't be read
 */
int rb_init64(vm_64* vm, const char* filename, uint32_t fileSize, args vmArgs){
    FILE* fp = fopen(filename,"r");
    BINARY_MW_TYPE a = 0; //useless value to store the magic word
    fread(&a, BINARY_MW_SIZE ,1,fp); //the magic word is ignored
    //stack init
    vm->registersStack = st_init();
    vm->functionsStack = st_init();
    st_push(vm->registersStack, vmArgs.argc);
    for(int i=0; i<vmArgs.argc; i++)
        st_push(vm->registersStack, (uint64_t) vmArgs.argv[i]);
    //flags init
    for(int i=0; i<FLAGS_NUMBER; i++)
        vm->flags[i] = false;
    //data init
    uint64_t nbData;
    fread(&nbData, 1, sizeof(uint64_t), fp);
    if(nbData)
        vm->data = malloc(sizeof(data_t) * nbData);
    for(uint64_t i=0; i<nbData; i++)
        fread(vm->data[i], 1, SIZE_DATA, fp);
    //code init
    vm->nombreInstruction = (fileSize- BINARY_MW_SIZE ) / sizeof(uint64_t);
    vm->nombreInstruction -= 1 + nbData * INST_PER_DATA;
    vm->code = malloc(sizeof(uint64_t) * vm->nombreInstruction);
    if(fread(vm->code,sizeof(uint64_t),vm->nombreInstruction,fp) == vm->nombreInstruction){
        fclose(fp);
        return READ_OK;
    }
    fclose(fp);
    return READ_NOT_OK;
}

