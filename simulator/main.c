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
        asrm* vm = asrm_init();
        bool loading = load_file(argv[1], vm);
        if(!loading){
            fprintf(stderr, "Unable to set the simulator up.\n");
            return RET_NO_VM;
        }
        asrm_run(vm);
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

