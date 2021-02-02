/*---------------------------------------------------------\
|This file constains macros and constants used to recognise|
|opperands and mnemonics.                                  |
\---------------------------------------------------------*/

#ifndef OPPERANDS
#define OPPERANDS

#include <string.h>
//opperands
#define SLP  0x00
#define SET  0x1
#define READ 0x2
#define CPY  0x3
#define ADD  0x4
#define SUB  0x5
#define AND  0x6
#define OR   0x7
#define XOR  0x8
#define NOT  0x9
#define LSL  0xA
#define LSR  0xB
#define EQ   0xC
#define LES  0xD
#define STR  0xE
#define LOAD 0xF
#define CC2  0x08
#define JIF  0x09
#define POP  0x0A
#define PUSH 0x0B
#define CALL 0x0C
#define RET  0x0D
#define QUIT 0x0E
#define DEBUG  0x0F
#define CMPNOT 0x01
#define RETINT 0x02
#define SETINT 0x04

#define SETINT_MASK 0xFC
#define isSETINT(inst) ((inst & SETINT_MASK) == SETINT)

#endif

