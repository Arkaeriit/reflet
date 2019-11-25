#include "link64.h"


/*
 * Initialise a list where we put the code from all object files.
 * return :
 *  a pointer to the list.
 */
codeHead* l64_initList(){
    codeHead* ret = malloc(sizeof(codeHead));
    ret->size = 0;
    ret->next = malloc(sizeof(head));
    return ret;
}

/*
 * add a line to a list
 * arguments :
 *      cH : the list
 *      texte : the line we want to add.
 */
void l64_addList(codeHead* cH,char* texte){
    code* elem = cH.next;
    for(int i=0; i<ch->size; i++)
        elem = elem->next;
    elem->next = malloc(sizeof(head));
    elem->code = texte;
}

/*
 * Get a line from the list
 * Arguments :
 *      cH : the list
 *      index : the index of the element we want to get
 * return :
 *      the structure representing the list. We can get the text by searching return->texte
 */
code* l64_getList(codeHead* cH,uint64_t index){
    if(index > liste->size || index < 1){
        printf("Erreur, mauvais indexage");
        exit(EXIT_FAILURE);
    }
    code* ret = cH->next;
    for(int i=1;i<index;i++){
        ret=ret->next;
    }
    return ret;
}

void l64_link(FILE** fin,FILE* fout){
    
