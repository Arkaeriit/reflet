#ifndef READBIN
#define READBIN

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "constants.h"
#include "stack.h"
#include <stdbool.h>

typedef char data_t[SIZE_DATA];

typedef struct vm_64_struct {
    uint64_t nombreInstruction;
    uint64_t* code;
    bool flags[FLAGS_NUMBER];
    uint64_t registers[REGISTERS_NUMBER];
    stack* registersStack;
    stack* functionsStack;
    data_t* data;
} vm_64;

#include "mainVM.h"

//Sert à la fonction rb_analyze
typedef struct infofile_struct {
    bool flagCorrect; //indique si le binaire peut être lisible
    bool flag64; //indque si le binaire est en 64 bits
    uint32_t fileSize; //indique la taille du fichier binaire
} infofile;

infofile rb_analyze(const char* filename);

int rb_init64(vm_64* vm,const char* filename,uint32_t fileSize,args vmArgs);

#endif

