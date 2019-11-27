#ifndef LINK64
#define LINK64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "constants.h"

/*
 * These two struct are ment to represent the code we must link as
 * a linked list.
 */
typedef struct code_struct {
    struct code_struct* next;
    uint8_t label; //a branching or an other instruction
    char* texte;
} code;

typedef struct codeHead_struct {
    uint64_t size;
    struct code_struct* next;
} codeHead;

codeHead* l64_initList();
void l64_addList(codeHead* cH,char* texte, uint8_t label);
code* l64_getList(codeHead* cH,uint64_t index);
//

int l64_addFile(codeHead* cH,FILE* fin);
void l64_link(FILE** fin,FILE* fout,int nFiles);

#endif

