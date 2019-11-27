#ifndef LINK64
#define LINK64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "constants.h"

//this struct is used as the link in the chain representing label
typedef struct label_tab_struct {
    char* name; 
    uint64_t* places;
    uint64_t position;
    uint64_t numberOfCalls;
    bool alreadyPlaced;
} label_tag;

/*
 * These two struct are ment to represent the code we must link as
 * a linked list.
 */
typedef struct code_struct {
    struct code_struct* next;
    uint8_t label; //a branching or an other instruction
    label_tag* tag; //only used in the chain used to store the labels
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
typedef codeHead labelHead;
bool l64_isLabelThere(labelHead* lB,const char* labelName, uint64_t* indx);
label_tag* l64_autoGetLabel(labelHead* lB, const char* labelName);
int l64_addLabel(labelHead* lB, const char* labelNamen, uint64_t position);
void l64_addCall(labelHead* lB, const char* labelName, uint64_t position);
//

int l64_addFile(codeHead* cH, labelHead* lB, FILE* fin);
int l64_branching(codeHead* cH, labelHead* lB);
void l64_link(FILE** fin,FILE* fout,int nFiles);

#endif

