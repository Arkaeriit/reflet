#include <stdio.h>
#include "loader.h"
#include "reflet.h"
#include "config.h"

static void help();

int main(int argc, char** argv){
    if(argc == 2 || argc == 3){
        if(argc == 2 && !(strcmp(argv[1], "help") && strcmp(argv[1], "--help") && strcmp(argv[1], "-h"))){
            help();
            return 0;
        }
        reflet* vm = reflet_init();
        if(argc == 3)
            applyConfig(vm, argv[2]);
        bool loading = load_file(argv[1], vm);
        if(!loading){
            fprintf(stderr, "Unable to set the simulator up.\n");
            return RET_NO_VM;
        }
        reflet_run(vm);
        reflet_free(vm);
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

