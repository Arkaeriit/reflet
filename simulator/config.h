

#ifndef CONFIG_L
#define CONFIG_L

#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "reflet.h"

#define SET_WORD_SIZE(vm)                                 \
    vm->config->word_size_byte = vm->config->word_size/8; \
    word_t mask = 0;                                      \
    for(word_t i=0; i<vm->config->word_size_byte; i++){   \
        mask = mask << 8;                                 \
        mask = mask + 0xFF;                               \
    }                                                     \
    vm->config->word_mask = mask                           


void applyConfig(reflet* vm, const char* configFile);

#endif

