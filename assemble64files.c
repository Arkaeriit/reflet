/*----------------------------------------------------------\
|This file contain functions used to compile lines relatives|
|to string and file manipulation.                           |
\----------------------------------------------------------*/

#include "assemble64files.h"

uint8_t a64f_open(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RRN){
        a64_createMachineCode(OPEN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction OPEN.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_close(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(CLOSE, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction CLOSE.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_stdin(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_R){
        a64_createMachineCode(STDIN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction STDIN.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_stdout(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_R){
        a64_createMachineCode(STDOUT, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction STDOUT.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_stderr(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_R){
        a64_createMachineCode(STDERR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction STDERR.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_gchar(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_R){
        a64_createMachineCode(GCHAR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction GCHAR.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_pchar(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_R){
        a64_createMachineCode(PCHAR, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction PCHAR.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_write(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RRR){
        a64_createMachineCode(WRITE, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction WRITE.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_read(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RRR){
        a64_createMachineCode(READ, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction READ.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_readline(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RRR){
        a64_createMachineCode(READ_LINE, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction READ_LINE.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_strlen(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(STRLEN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction STRLEN.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_strcmp(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(STRCMP, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction STRCMP.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_eofcmp(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_R){
        a64_createMachineCode(EOFCMP, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction EOFCMP.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_stn(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(STN, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction STN.\n");
    return COMPILED_LINE_NOT_OK;
}

uint8_t a64f_nts(char** elems, uint8_t n, uint64_t* fullCode){
    uint8_t config = a64_analyzeLine(elems, n);
    if(config == COMPILE_RR){
        a64_createMachineCode(NTS, elems, n, fullCode);
        return COMPILED_LINE_INSTRUCTION;
    }
    fprintf(stderr, "    Error : wrong argument for instruction NTS.\n");
    return COMPILED_LINE_NOT_OK;
}

