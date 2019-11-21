#include "assemble64.h"

void a64_assemble(FILE* fin,FILE* fout){
    char* line = malloc(255);
    char** elems = malloc(sizeof(char*) * 150); //We will never get 150 words
    fgets(line,255,fin);
    while(line != NULL && strlen(line) > 2){
        uint8_t n = aXX_preprocessLine(line,elems);

        aXX_freeElems(n,elems);
        fgets(line,255,fin);
    }
    
}


