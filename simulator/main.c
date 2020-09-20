#include <stdio.h>
#include "loader.h"
#include "asrm.h"

static void help();

int main(int argc, char** argv){
    if(argc == 2){
        if(!(strcmp(argv[1], "help") && strcmp(argv[1], "--help") && strcmp(argv[1], "-h"))){
            help();
            return 0;
        }
        asrm* vm = load_file(argv[1]);
        if(vm == NULL){
            fprintf(stderr, "Unable to set the simulator up.\n");
            return RET_NO_VM;
        }
        //do stuff here
        asrm_free(vm);
    }else{
        fprintf(stderr, "Error: invalid arguments.\n");
        help();
        return RET_INVALID_ARGS;
    }
    return 0;
}

static void help(){
    puts("Help message here");
}

