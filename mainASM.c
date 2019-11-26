/*---------------------------------------------\
|This file is the main for the executable      |
|that will launch the assembler and the linker.|
\---------------------------------------------*/

#include "mainASM.h"

int main(int argc,char** argv){
    if(!strcmp(*(argv+1),"compile") && argc > 2){
        FILE *fin,*fout;
        fin = fopen(*(argv+2),"r");
        if(fin == NULL){
            fprintf(stderr,"Error : can not read file %s.\n",*(argv+2));
            return 2;
        }
        //choosing output file
        if(argc > 3){
            fout = fopen(*(argv+3),"w");
        }else{
            char* extention = *(argv+2) + strlen(*(argv+2)) - 4; //4 last chars of the 2nd argument
            char* inputCopie = malloc(strlen(*(argv+2)) + 4);
            strcpy(inputCopie,*(argv+2));
            if(!strcmp(extention,".asa")){
                inputCopie[strlen(inputCopie) - 1] = 'o';//we replace the last char by 0
                fout = fopen(inputCopie,"w");
            }else{
                strcat(inputCopie,".aso");
                fout = fopen(inputCopie,"w");
            }
            free(inputCopie);
        }
        if(fout == NULL){
            fprintf(stderr,"Error : can not write on file %s.\n",*(argv+3));
            return 2;
        }
        a64_assemble(fin,fout);
    }else if(!strcmp(*(argv+1),"link") && argc > 3){
        FILE **fin,*fout;
        fin = malloc(sizeof(argc - 3));
        for(int i=0; i<argc-3; i++){
            fin[i] = fopen(*(argv+2+i),"r");
            if(fin == NULL){
                fprintf(stderr,"Error : can not read file %s.\n",*(argv+2+i));
                return 2;
            }
        }
        l64_link(fin,fout);
    }else{
        quickHelp();
        return 1;
    } 

    return 0;
}

void quickHelp(){
    fprintf(stderr,"Not enouth args.\n");
}

