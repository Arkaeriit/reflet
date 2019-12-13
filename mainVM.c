/*------------------------------------------\
|This file contain what is needed to launch |
the vm and interprete a binary file.        |
\------------------------------------------*/

#include "mainVM.h"

int main(int argc,char** argv){
    if(argc < 2){
        fprintf(stderr, "Not enough arguments.\nUsage : asvm [file] [arguments]...\n");
        return 1;
    }
    char* filename = argv[1];
    
    infofile ifo = rb_analyze(filename);
    if(ifo.flagCorrect){
        if(ifo.flag64){
            vm_64 vm;
            args vmArgs = mVM_decodeArgs(argc, argv);
            if(rb_init64(&vm, filename, ifo.fileSize, vmArgs)){
                fprintf(stderr,"Error while initialising the VM.\n");
                return EXECUTE_ERROR_VM;
            }
            return e64_execute(&vm);
        }
    }else{
        fprintf(stderr,"Error : %s is not a valid file\n",filename);
        return EXECUTE_INVALID_FILE;
    }
    return EXECUTE_ERROR_VM;
}

/*
 * Return a struct made to give the arguments passed to the asvm when
 * initialising the vm
 *  Arguments :
 *      the sames ones that the main function
 */
args mVM_decodeArgs(int argc, char** argv){
    args ret;
    ret.argc = argc - 1;
    ret.argv = argv + 1;
    return ret;
}

