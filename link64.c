/*-------------------------------------------------------------------\
||This file contain the function used to link various object files   |
|into a single binary file. It's main task is to put the instruction |
|one after an other and handle lables used for function call or other|
|branching. If the next line is a simple instruction it will be a    |
|'i' followed by 64 bits of instruction. If it is the declaration    |
|of a new label it will be a 'j' folowed by a 'd' followed by two    |
|blank bytes followed by a the name of the label on 150 bytes. If    |
|it is a branching or a function call it will be a 'j' followed      |
|by a 'j' followed by two bytes used to know the opperand of the     |
|reulting instruction, followed by the name of the label where to    |
|jump.                                                               |
\-------------------------------------------------------------------*/

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
    elem->tag = malloc(sizeof(label_tag)); //let us used the chain to add label
    cH->size++;
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
    if(index > cH->size || index < 0){
        printf("Erreur, mauvais indexage\n");
        exit(EXIT_FAILURE);
    }
    struct code_struct* ret = cH->next;
    for(int i=0;i<index;i++){
        ret=ret->next;
    }
    return ret;
}

/*
 * Indicate if a label is on the list and can return it's index
 *  Arguments :
 *      lB : the list with the labels
 *      labelName : name of the label we search
 *      index : if not NULL and if we fond the label we put it's index here
 *  return : 
 *      true if the label we search is on the list
 *      false if it is not
 */
bool l64_isLabelThere(labelHead* lB,const char* labelName,uint64_t* index){
    code* elem = lB->next;
    for(uint64_t i=0; i<lB->size; i++){
        if(!strcmp(labelName,elem->tag->name)){
            if(index != NULL)
                *index = i;
            return true;
        }
        elem = elem->next;
    }
    return false;
}

/*
 * Get the tag of a label, if it is a new label it will create it's tag
 *  Arguments :
 *      lB : the chain where the label are stored
 *      labelName : the ame of the label
 *  return :
 *      the tag of the label
 */
label_tag* l64_autoGetLabel(labelHead* lB, const char* labelName){
    uint64_t index;
    if(l64_isLabelThere(lB, labelName, &index))
        return l64_getList(lB,index)->tag;
    l64_addList(lB, NULL,0);
    label_tag* ret = l64_getList(lB, lB->size - 1)->tag;
    ret->name = malloc(sizeof(char) * MAX_SIZE_LABEL);
    strcpy(ret->name, labelName);
    ret->places = malloc(sizeof(uint64_t) * MAX_CALL_NUMBER);
    ret->numberOfCalls = 0;
    return ret;
}

/*
 * Indicate that we are defining a new label
 *  Arguments : 
 *      lB : the chain with the labels
 *      labelName : the name of the label
 *      position : the position of the label in the linked code
 *  return :
 *      LINK_ERROR_DOUBLE_DEFINITION if a label with the same name is already placed
 *      LABEL_OK otherwise
 */
int l64_addLabel(labelHead* lB,const char* labelName, uint64_t position){
    label_tag* tag = l64_autoGetLabel(lB, labelName);
    if(tag->alreadyPlaced){
        return LINK_ERROR_DOUBLE_DEFINITION;
    }
    tag->alreadyPlaced = true;
    tag->position = position;
    return LABEL_OK;
}

/*
 * Indicate that we are defining a new data label
 *  Arguments : 
 *      dT : the chain with the labels
 *      labelName : the name of the label
 *      position : the position of the label in the linked code
 *      data : the content of the data
 *  return :
 *      LINK_ERROR_DOUBLE_DEFINITION if a label with the same name is already placed
 *      LABEL_OK otherwise
 */
int l64_addData(dataHead* dT,const char* labelName, char* content){
    l64_addLabel(dT, labelName, dT->size + 1);
    label_tag* tag = l64_autoGetLabel(dT, labelName);
    tag->content = content;
    return LABEL_OK;
}

/*
 * Indicate that we will call a label from a position
 *  Arguments :
 *      lB : the chain with the labels
 *      labelName : the name of the label
 *      position : the position from where the label is called
 */
void l64_addCall(labelHead* lB, const char* labelName, uint64_t position){
    label_tag* tag = l64_autoGetLabel(lB, labelName);
    tag->places[tag->numberOfCalls] = position;
    tag->numberOfCalls++;
}

/*
 * Indicate that we will load data from that position
 *  Arguments :
 *      dT : the chain with the data
 *      labelName : the name of the label
 *      position : the position from where the label is called
 */
void l64_loadData(dataHead* dT, const char* labelName, uint64_t position){
    l64_addCall(dT, labelName, position); //Might change later
}

/*
 * Add a first executed line of code into the list of instruction which will jump to a function named start
 *  Argument :
 *      cH : the list of instruction
 *      lB : the list of label for jump
 */
void l64_firstInst(codeHead* cH, labelHead* lB){
    char* opperand = malloc(sizeof(uint64_t));
    *((uint64_t*) opperand) = JMP;
    l64_addCall(lB, "start", cH->size);
    l64_addList(cH, opperand, 'd');
}

/*
 * Add a file to the list representing all the code.
 *  Arguments : 
 *      cH : the list
 *      lB : the list with info concerning the labels
 *      dT : the list with info concerning data
 *      fin : a pointer to the file we must add
 *  return :
 *      LINK_OK if everything went well
 *      an error code otherwise
 */
int l64_addFile(codeHead* cH, labelHead* lB, dataHead* dT, FILE* fin){
    OBJECT_MW_TYPE magicWord;
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
            char* labelName = malloc(sizeof(char) * MAX_SIZE_LABEL);
            char* opperand = malloc(sizeof(uint64_t)); //The start of the line of machine code
            fread(&bufferStart, 1, 1, fin);
            fread(opperand, 1, 2, fin);
            fread(labelName, 1, MAX_SIZE_LABEL, fin);
            if(bufferStart == 'd'){
                l64_addLabel(lB, labelName, cH->size);
            }else if(bufferStart == 'j'){
                l64_addCall(lB, labelName, cH->size);
                l64_addList(cH, opperand, 'j');
            }else{
                return LINK_INVALID_LABEL;
            }
        }else if(bufferStart == 'd'){
            char* labelName = malloc(sizeof(char) * MAX_SIZE_LABEL);
            char* opperand = malloc(sizeof(uint64_t)); //The start of the line of machine code
            fread(&bufferStart, 1, 1, fin);
            fread(opperand, 1, 2, fin);
            fread(labelName, 1, MAX_SIZE_LABEL, fin);
            char* data = malloc(sizeof(char) * SIZE_DATA);
            if(bufferStart == 'd'){ //a new data label is defined 
                fread(data, 1, SIZE_DATA, fin);
                l64_addData(dT, labelName, data);
            }else if(bufferStart == 'l'){
                char reg;
                fread(&reg, 1, 1, fin); //We read the register where we put the adress of the data
                *((uint64_t*) opperand) |= (reg << ARG_SHIFT_1); //We put the register after the opperand
                l64_loadData(dT, labelName, cH->size);
                l64_addList(cH, opperand, 'd');
                fread(data, 1, SIZE_DATA - 1, fin); //We ignore the rest of the line
                free(data);
            }else{
                return LINK_INVALID_LABEL;
            }
        }else{
            return LINK_INVALID_LABEL;
        }
    }
    return LINK_OK;
}

/*
 * Complete all the left alone lines about branching in an about-to-be-linked code.
 *  Arguments :
 *      cH : the list with the machine code
 *      lB : the list with the labels for jumps or data
 *      shift : the shift between the opperand already in cH and 
 *              the info in lB. This should be ARG_SHIFT_1 if lB is
 *              typed as labelHead and ARG_SHIFT_2 if l is typed as
 *              dataHead
 *  return :
 *      LINK_OK if everything went well
 *      an error code otherwise
 */
int l64_branching(codeHead* cH, labelHead* lB, uint64_t shift){
    for(uint64_t i=0; i<lB->size; i++){
        label_tag* tag = l64_getList(lB, i)->tag;
        if(!tag->alreadyPlaced){
            fprintf(stderr, "Error : the label %s is called but not defined.\n",tag->name);
            return LINK_UNDEFINED_LABEL;
        }
        for(uint64_t i=0; i<tag->numberOfCalls; i++){
            uint64_t* texte = (uint64_t*) l64_getList(cH, tag->places[i])->texte; //The line of code that need completion
            *texte |= (tag->position << shift);
        }
    }
    return LINK_OK;
}

/*
 * Write the content data to be loaded into the output file
 *  Arguments:
 *      dT : the lis with all the data
 *      fout : the output file
 */
void l64_writingData(dataHead* dT, FILE* fout){
    fwrite(&(dT->size), 1, sizeof(uint64_t), fout); //We write the total oumout af data to be loaded
    for(uint64_t i=0; i<dT->size; i++){
        label_tag* tag = l64_getList(dT, i)->tag;
        fwrite(tag->content, 1, SIZE_DATA, fout);
    }
}

/*
 * Link object files toogether
 *  Arguments :
 *      fin : an array of input files
 *      fout : the output file
 *      nFiles : the size of fin
 */
int l64_link(FILE** fin,FILE* fout, int nFiles){
    codeHead* cH = l64_initList();
    labelHead* lB = l64_initList();
    dataHead* dT = l64_initList();

    l64_firstInst(cH, lB);

    for(int i=0; i<nFiles; i++){
        if(l64_addFile(cH, lB, dT, fin[i]) != LINK_OK){
            fprintf(stderr, "Error : invalid object file(s).\n");
            return LINK_INVALID_FILE;
        }
    }

    if(l64_branching(cH, lB, ARG_SHIFT_1) != LINK_OK){
        return LINK_UNDEFINED_LABEL;
    }
    if(l64_branching(cH, dT, ARG_SHIFT_2) != LINK_OK){
        return LINK_UNDEFINED_LABEL;
    }   
    
    BINARY_MW_TYPE magicWord = BINARY_MW;
    fwrite(&magicWord, 1, BINARY_MW_SIZE ,fout);
    l64_writingData(dT, fout);
    for(uint64_t i=0; i<cH->size;i++){ //We write the compiled code in the output file
        code* line = l64_getList(cH,i);
        fwrite(line->texte, 1, sizeof(uint64_t), fout);
    }
    return LINK_OK;
}

