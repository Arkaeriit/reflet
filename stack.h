#ifndef STACK
#define STACK

#include <stdlib.h>
#include <inttypes.h>

typedef struct stack_elem_struct {
    struct stack_elem_struct* previous;
    uint64_t elem;
} stack_elem;

typedef stack_elem* stack;

stack* st_init();
void st_push(stack* st, uint64_t elem);
uint64_t st_pull(stack* st);

#endif

