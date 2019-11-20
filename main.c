#include "ReadBin.h"
#include "Exec64.h"

int main(int argc,char** argv){
    char* filename = "test.bin";
    
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

