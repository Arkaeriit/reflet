#ifndef READBIN
#define READBIN

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "constants.h"
#include "stdbool.h"

typedef struct vm_64_struct {
    uint64_t nombreInstruction;
    uint64_t* code;
    bool* flags;
    uint64_t* registers;
} vm_64;

//Sert à la fonction rb_analyze
typedef struct infofile_struct {
    bool flagCorrect; //indique si le binaire peut être lisible
    bool flag64; //indque si le binaire est en 64 bits
    uint32_t fileSize; //indique la taille du fichier binaire
} infofile;

infofile rb_analyze(const char* filename);

int rb_init64(vm_64* vm,const char* filename,uint32_t fileSize);

#endif

