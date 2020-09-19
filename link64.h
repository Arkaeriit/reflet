#ifndef LINK64
#define LINK64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "constants.h"

/*
 * these two structs are ment to represnent the data dans positions
 * in the code we must link as an arraylist
 */
//this struct is used as the link in the chain representing label for jumps
typedef struct label_tab_struct {
    char* name; 
    uint64_t* places;
    uint64_t position; 
    uint64_t numberOfCalls;
    bool alreadyPlaced;
    char* content; //used for adding data labels
} label_tag;

typedef struct labelHead_struct {
    uint64_t size;
    label_tag** labels;
} labelHead;

typedef labelHead dataHead;

labelHead* l64_initLabel();
label_tag* l64_newLabel(labelHead* lB);
label_tag* l64_getLabel(labelHead* lB, uint64_t index);
/*
 * These two struct are ment to represent the code we must link as
 * a linked list.
 */
typedef struct code_struct {
    struct code_struct* next;
    char label;
    char* texte;
} code;

typedef struct codeHead_struct {
    uint64_t size;
    struct code_struct* next;
    struct code_struct* last; //a quick reference to the last element
} codeHead;

codeHead* l64_initList();
void l64_addList(codeHead* cH,char* texte, uint8_t label);
code* l64_getList(codeHead* cH,uint64_t index);
//
bool l64_isLabelThere(labelHead* lB,const char* labelName, uint64_t* indx);
label_tag* l64_autoGetLabel(labelHead* lB, const char* labelName);
int l64_addLabel(labelHead* lB, const char* labelNamen, uint64_t position);
void l64_addCall(labelHead* lB, const char* labelName, uint64_t position);
int l64_addData(dataHead* dT, const char* labelNamen, char* content);
void l64_loadData(dataHead* dT, const char* labelName, uint64_t position);
//

void l64_firstInst(codeHead* cH, labelHead* lB);
int l64_addFile(codeHead* cH, labelHead* lB, dataHead* dT, FILE* fin);
int l64_branching(codeHead* cH, labelHead* lB, uint64_t shift);
void l64_writingData(dataHead* dT, FILE* fout);
int l64_link(FILE** fin,FILE* fout,int nFiles);

bool l64_sameString(const char* string1, const char* string2);
size_t l64_strlen(const char* string1);

#endif

