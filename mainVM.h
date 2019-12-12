#ifndef MAINVM
#define MAINVM

typedef struct args_struct { //This struc is ment to give info about the arguments passed to the asvm
    int argc;
    char** argv;
} args;

args mVM_decodeArgs(int argc,char** argv);

#include "ReadBin.h"
#include "Exec64.h"

#endif


