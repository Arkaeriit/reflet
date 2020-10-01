#include <stdio.h>
#include "pico_assembler.h"

static void help();

int main(int argc, char** argv){
    if(argc == 2){
        if(!(strcmp(argv[1], "help") && strcmp(argv[1], "--help") && strcmp(argv[1], "-h"))){
            help();
            return 0;
        }else{
        fprintf(stderr, "Error: invalid arguments.\n");
            help();
            return RET_INVALID_ARGS;
        }
    }else if(argc == 3){
        mini_assembleFile(argv[1], argv[2]);
    }else{
        fprintf(stderr, "Error: invalid arguments.\n");
        help();
        return RET_INVALID_ARGS;
    }
    return 0;
}

static void help(){
    puts("asrmpas : the asrm pico-assembler\n"
         "Usage : asrmpasm <assembly file> <output file>"
    );
}

