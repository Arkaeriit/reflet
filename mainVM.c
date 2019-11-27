/*------------------------------------------\
|This file contain what is needed to launch |
the vm and interprete a binary file.        |
\------------------------------------------*/

#include "ReadBin.h"
#include "Exec64.h"

int main(int argc,char** argv){
    if(argc < 2){
        return 1;
    }
    char* filename = argv[1];
    
    infofile ifo = rb_analyze(filename);
    if(ifo.flagCorrect){
        if(ifo.flag64){
            vm_64 vm;
            if(rb_init64(&vm,filename,ifo.fileSize)){
                fprintf(stderr,"Error while initialising the VM.\n");
                return 2;
            }
            e64_execute(&vm);
        }
    }else{
        fprintf(stderr,"Error : %s is not a valid file\n",filename);
        return 1;
    }
    return 0;
}

