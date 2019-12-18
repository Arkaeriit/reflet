#ifndef ASSEMBLE64FILES
#define ASSEMBLE64FILES

#include "assembleXX.h"
#include "assemble64.h"
#include "stdio.h"

uint8_t a64f_open(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_close(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_stdin(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_stdout(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_stderr(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_gchar(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_pchar(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_write(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_read(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_readline(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_strlen(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_strcmp(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_eofcmp(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_stn(char** elems, uint8_t n, uint64_t* fullCode);
uint8_t a64f_nts(char** elems, uint8_t n, uint64_t* fullCode);

#endif

