/*-------------------------------------------------------------\
|This file is used to load files into the RAM of the simulator.|
\-------------------------------------------------------------*/

#ifndef LOADER
#define LOADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "reflet.h"

bool load_file(const char* filename, reflet* vm, bool ignore_first_line);

#endif

