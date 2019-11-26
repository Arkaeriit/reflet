#include "link64.h"


/*
 * Initialise a list where we put the code from all object files.
 * return :
 *  a pointer to the list.
 */
codeHead* l64_initList(){
    codeHead* ret = malloc(sizeof(codeHead));
    ret->size = 0;
    ret->next = malloc(sizeof(struct code_struct));
    return ret;
}

/*
 * add a line to a list
 * arguments :
 *      cH : the list
 *      texte : the line we want to add.
 */
void l64_addList(codeHead* cH,char* texte, uint8_t label){
    struct code_struct* elem = cH->next;
    for(int i=0; i<cH->size; i++)
        elem = elem->next;
    elem->next = malloc(sizeof(struct code_struct)); //We assume that the next element of a line is allocated but not initialized
    elem->texte = texte;
    elem->label = label;
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
    if(index > cH->size || index < 1){
        printf("Erreur, mauvais indexage");
        exit(EXIT_FAILURE);
    }
    struct code_struct* ret = cH->next;
    for(int i=1;i<index;i++){
        ret=ret->next;
    }
    return ret;
}

/*
 * Add a file to the list representing all the code.
 *  Arguments : 
 *      cH : the list
 *      fin : a pointer to the file we must add
 *  return :
 *      LINK_OK if everything went well
 *      an error code otherwise
 */
int l64_addFile(codeHead* cH, FILE* fin){
    uint64_t magicWord;
    fread(&magicWord, 1, OBJECT_MW_SIZE ,fin);
    if(magicWord != OBJECT_MW){
        return LINK_INVALID_FILE;
    }
    uint8_t bufferStart; //Used to read the byte before each instruction
    while(fread(&bufferStart, 1, 1, fin) == 1){ //There is an idication to read
        if(bufferStart == 'i'){
            char* instruction = malloc(sizeof(uint64_t));
            fread(instruction, 1, sizeof(uint64_t), fin);
            l64_addList(cH, instruction, 'i');
        }else if(bufferStart == 'j'){
            //todo
        }else{
            return LINK_INVALID_LABEL;
        }
    }
    return LINK_OK;
}

void l64_link(FILE** fin,FILE* fout){}
    
