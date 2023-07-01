/*---------------------------------------------------------\
|This file constains macros and constants used to recognise|
|opperands and mnemonics.                                  |
\---------------------------------------------------------*/

#ifndef OPPERANDS
#define OPPERANDS

#include <string.h>
//opperands
#define READ 0x0
#define CPY  0x1
#define SET  0x2
#define ADD  0x3
#define AND  0x4
#define OR   0x5
#define XOR  0x6
#define NOT  0x7
#define LSL  0x8
#define LSR  0x9
#define EQ   0xA
#define LES  0xB
#define STR  0xC
#define LOAD 0xD
#define JIF  0xE0
#define CALL 0xE1
#define RET  0xE2
#define POP  0xE3
#define PUSH 0xE4
#define CC2  0xE5
#define CMPNOT 0xE6
#define TBM    0xE7
#define QUIT   0xE8
#define DEBUG  0xE9
#define ATOM   0xEA
#define RETINT 0XEB
#define SETINT 0xEC
#define GETINT 0xF0
#define GETINTSTACK 0xF4
#define SETINTSTACK 0xF8
#define SOFTINT     0xFC

#define SETINT_MASK 0xFC
#define isSETINT(inst) ((inst & SETINT_MASK) == SETINT)
#define isGETINT(inst) ((inst & SETINT_MASK) == GETINT)
#define isGETINTSTACK(inst) ((inst & SETINT_MASK) == SETINTSTACK)
#define isSETINTSTACK(inst) ((inst & SETINT_MASK) == GETINTSTACK)
#define isSOFTINT(inst)     ((inst & SETINT_MASK) == SOFTINT)

#endif

