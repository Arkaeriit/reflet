#ifndef LINK64
#define LINK64

#include <stdio.h>
#include <string.h>


/*
 * These two struct are ment to represent the code we must link as
 * a linked list.
 */
 
#define SIZELINE 255
 
typedef struct codeHead_struct {
    uint64_t size;
    code* next;
} codeHead;

typedef struct code_sruct {
    code* next;
    char* texte;
} code;

codeHead* l64_initList();
void l64_addList(codeHead* cH,char* texte);
code* l64_getList(codeHead* cH,uint64_t index);
//

void l64_link(FILE** fin,FILE* fout);

#endif

