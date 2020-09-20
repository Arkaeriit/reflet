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
#include "asrm.h"

asrm* load_file(const char* filename);

#endif

