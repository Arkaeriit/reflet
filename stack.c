#include "stack.h"

stack* st_init(){
    stack* ret = malloc(sizeof(stack));
    *ret = NULL;
    return ret;
}

void st_push(stack* st, uint64_t elem){
    stack_elem* ste = *st;
    stack_elem* new = malloc(sizeof(stack_elem));
    new->previous = ste;
    new->elem = elem;
    *st = new;
}

uint64_t st_pull(stack* st){
    stack_elem* top = *st;
    uint64_t ret = top->elem;
    *st = top->previous;
    free(top);
    return ret;
}

