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
    ret->last = ret->next;
    return ret;
}

/*
 * Initialise an arraylist where we put info obout labels and data
 *  return :
 *      a pointer to a struct whith the size and the array
 */
labelHead* l64_initLabel(){
    labelHead* ret = malloc(sizeof(labelHead));
    ret->size = 0;
    ret->labels = malloc(sizeof(label_tag*)); //Even if we don't use this we do a malloc to make the free from  the newLabel function go smoothly
    return ret;
}

/*
 * Increase the size of the array list and give a pointer to 
 * the newly created element
 *  Arguments :
 *      lB : a pointer to the arraylist
 *  return :
 *      a pointer to the last element of the array after expansion
*/
label_tag* l64_newLabel(labelHead* lB){
    label_tag** tmp = malloc(sizeof(label_tag*) * (lB->size + 1)); //We create a new array that is one element bigger than the last
    for(uint64_t i=0; i<lB->size; i++){
        tmp[i] = lB->labels[i];
        //printf("%s\n",tmp[i]->name);
    }
    free(lB->labels);
    lB->labels = tmp;
    lB->size++;
    tmp[lB->size - 1] = malloc(sizeof(label_tag));
    label_tag* ret = tmp[lB->size - 1];
    return ret;
}

/*
 * Get a label tag from the array list
 *  Arguments :
 *      lB : the arraylist
 *      index : the index of the element we want
 *  return :
 *      the desired element
 */
label_tag* l64_getLabel(labelHead* lB, uint64_t index){
    if(index > lB->size || index < 0){
        printf("Erreur, mauvais indexage\n");
        exit(EXIT_FAILURE);
    }
    return lB->labels[index];
}

/*
 * add a line to a list
 * arguments :
 *      cH : the list
 *      texte : the line we want to add.
 */
void l64_addList(codeHead* cH,char* texte, uint8_t label){
    //printf("addList\n");
    //printf("%c %s\n", label, texte);
    //struct code_struct* elem = l64_getList(cH, cH->size);
    //struct code_struct* elem = cH->next;
    //for(int i=0; i<cH->size; i++)
    //    elem = elem->next;
    struct code_struct* elem = cH->last;
    elem->next = malloc(sizeof(struct code_struct)); //We assume that the next element of a line is allocated but not initialized
    elem->texte = texte;
    elem->label = label;
    cH->size++;
    cH->last = elem->next; //We update the last element
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
    //printf("getList\n");
    if(index > cH->size || index < 0){
        //printf("Erreur, mauvais indexage\n");
        exit(EXIT_FAILURE);
    }
    struct code_struct* ret = cH->next;
    //printf("fetch : %lu/%lu\n",index, cH->size);
    for(uint64_t i=0;i<index;i++){
        
    //printf("fetch : %lu/%lu i : %lu\n",index, cH->size, i);
        ret=ret->next;
    }
    //printf("fetched\n");
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
    //printf("IsLabelThere - size : %lu\n", lB->size);
    uint64_t loopLimit = lB->size;
    for(uint64_t i=0; i<loopLimit; i++){
            //printf("l64_isLabelThere - loop ");
            //printf("; string : %s ;",lB->labels[i]->name);
            //printf("label name : %s ",labelName);
            //printf("; size of strings : %lu %lu\n",strlen(lB->labels[i]->name),strlen(labelName));
            bool cmp = l64_sameString(labelName,lB->labels[i]->name);
            //printf("isLabelThere - comparaison done\n");
        if(cmp){
            //printf("l64_isLabelThere - label found\n");
            if(index != NULL)
                *index = i;
            return true;
        }
            //printf("l64_isLabelThere - Endloop ");
    }
    return false;
}

/*
 * Check if two strings are the same or not
 *  Arguments:
 *      two strings to compare
 *  return:
 *      true if they are the same, false if they are different
 */
bool l64_sameString(const char* string1,const char* string2){
    //printf("sameString\n");
    size_t len1 = l64_strlen(string1);
    size_t len2 = l64_strlen(string2);
    //printf("sameString - size loaded ; size1 = %lu ; size2 = %lu\n",len1,len2);
    if(len1 != len2){
        //printf("sameString - false\n");
        return false;
    }
    //printf("sameString - comparaisons ; size = %lu\n",len1);
    for(size_t i=0; i<len1; i++)
        if(string1[i] != string2[i])
            return false;
    return true;
}

size_t l64_strlen(const char* string1){
    size_t ret = 0;
    while(string1[ret])
        ret++;
    return ret;
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
    //printf("autoGetlabel1\n");
    if(l64_isLabelThere(lB, labelName, &index))
    //printf("autoGetlabel2\n");
        return l64_getLabel(lB,index);
    //printf("autoGetlabel2 - OK\n");}
    //printf("autoGetlabel3\n");
    label_tag* ret = l64_newLabel(lB);
    //printf("autoGetlabel4\n");
    ret->name = malloc(sizeof(char) * MAX_SIZE_LABEL);
    strcpy(ret->name, labelName);
    //printf("auto get label : new string is named : %s\n",ret->name);
    ret->places = malloc(sizeof(uint64_t) * MAX_CALL_NUMBER);
    ret->numberOfCalls = 0;
    ret->alreadyPlaced = false;
    return ret;
}

/*
 * Indicate that we are defining a new label from the code.
 * The label indicate a position in the code.
 *  Arguments : 
 *      lB : the chain with the labels
 *      labelName : the name of the label
 *      position : the position of the label in the linked code
 *  return :
 *      LINK_ERROR_DOUBLE_DEFINITION if a label with the same name is already placed
 *      LABEL_OK otherwise
 */
int l64_addLabel(labelHead* lB,const char* labelName, uint64_t position){
    //printf("addlabel\n");
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
    //printf("addData\n");
    l64_addLabel(dT, labelName, dT->size);
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
    //printf("addCall\n");
    label_tag* tag = l64_autoGetLabel(lB, labelName);
    //printf("addCall - tag get\n");
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
            //printf("Instruction\n");
            char* instruction = malloc(sizeof(uint64_t));
            fread(instruction, 1, sizeof(uint64_t), fin);
            l64_addList(cH, instruction, 'i');
        }else if(bufferStart == 'j'){
            //printf("Jump\n");
            char* labelName = malloc(sizeof(char) * MAX_SIZE_LABEL);
            char* opperand = malloc(sizeof(uint64_t)); //The start of the line of machine code
            fread(&bufferStart, 1, 1, fin);
            fread(opperand, 1, 2, fin);
            fread(labelName, 1, MAX_SIZE_LABEL, fin);
            if(bufferStart == 'd'){
                //printf("define\n");
                l64_addLabel(lB, labelName, cH->size);
            }else if(bufferStart == 'j'){
                //printf("goto\n");
                l64_addCall(lB, labelName, cH->size);
                //printf("goto - call added\n");
                l64_addList(cH, opperand, 'j');
            }else{
                return LINK_INVALID_LABEL;
            }
        }else if(bufferStart == 'd'){
            //printf("data\n");
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
    //printf("branching\n");
    for(uint64_t i=0; i<lB->size; i++){
        label_tag* tag = l64_getLabel(lB, i);
        if(!tag->alreadyPlaced){
            fprintf(stderr, "Error : the label %s is called but not defined.\n",tag->name);
            return LINK_UNDEFINED_LABEL;
        }
        for(uint64_t i=0; i<tag->numberOfCalls; i++){
            //printf("elem : %s ; place %lu\n",tag->name, tag->position);
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
    //printf("writData\n");
    fwrite(&(dT->size), 1, sizeof(uint64_t), fout); //We write the total oumout af data to be loaded
    for(uint64_t i=0; i<dT->size; i++){
        label_tag* tag = l64_getLabel(dT, i);
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
    labelHead* lB = l64_initLabel();
    dataHead* dT = l64_initLabel();

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
    
    //fwrite(BINARY_MW_STRING, 1, BINARY_MW_SIZE ,fout);
    printf("\nmw written\n");
    //l64_writingData(dT, fout);
    for(uint64_t i=0; i<cH->size;i++){ //We write the compiled code in the output file
        code* line = l64_getList(cH,i);
        //fwrite(line->texte, 1, sizeof(uint64_t), fout);
        printf("\ncode written\n");
    }
    printf("all written\n");
    for(int i=0; i<nFiles; i++)
        fclose(fin[i]);
    free(fin);
    fclose(fout);
    printf("returning\n");
    return LINK_OK;
}

