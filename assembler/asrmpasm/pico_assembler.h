/*--------------------------------------\
|This file contain most of the functions|
|used by the basic assembler.           |
\--------------------------------------*/

#ifndef MINI_ASM
#define MINI_ASM

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

void mini_assembleFile(const char* fileIn, const char* fileOut);

#define RET_INVALID_ARGS 1
#define RET_UNOPEN_FILE 3
#define LINE_BUFF 10000

//opperands
#define isSlp(instuction) ((instuction & 0xF8) == 0)
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
#define JMP  0x08
#define JIF  0x09
#define POP  0x0A
#define PUSH 0x0B
#define CALL 0x0C
#define RET  0x0D
#define QUIT 0x0E
#define DEBUG  0x0F
#define CMPNOT 0x01
#define RETINT 0X02
//0x03
#define SETINT_MASK 0x04

//mnemonics
#define mnSLP(mn) !(strcmp(mn, "SLP") && strcmp(mn, "slp"))
#define mnSET(mn) !(strcmp(mn, "SET") && strcmp(mn, "set"))
#define mnREAD(mn) !(strcmp(mn, "READ") && strcmp(mn, "read"))
#define mnCPY(mn) !(strcmp(mn, "CPY") && strcmp(mn, "cpy"))
#define mnADD(mn) !(strcmp(mn, "ADD") && strcmp(mn, "add"))
#define mnSUB(mn) !(strcmp(mn, "SUB") && strcmp(mn, "sub"))
#define mnAND(mn) !(strcmp(mn, "AND") && strcmp(mn, "and"))
#define mnOR(mn) !(strcmp(mn, "OR") && strcmp(mn, "or"))
#define mnXOR(mn) !(strcmp(mn, "XOR") && strcmp(mn, "xor"))
#define mnNOT(mn) !(strcmp(mn, "NOT") && strcmp(mn, "not"))
#define mnLSL(mn) !(strcmp(mn, "LSL") && strcmp(mn, "lsl"))
#define mnLSR(mn) !(strcmp(mn, "LSR") && strcmp(mn, "lsr"))
#define mnEQ(mn) !(strcmp(mn, "EQ") && strcmp(mn, "eq"))
#define mnLES(mn) !(strcmp(mn, "LES") && strcmp(mn, "les"))
#define mnSTR(mn) !(strcmp(mn, "STR") && strcmp(mn, "str"))
#define mnLOAD(mn) !(strcmp(mn, "LOAD") && strcmp(mn, "load"))
#define mnJMP(mn) !(strcmp(mn, "JMP") && strcmp(mn, "jmp"))
#define mnJIF(mn) !(strcmp(mn, "JIF") && strcmp(mn, "jif"))
#define mnPOP(mn) !(strcmp(mn, "POP") && strcmp(mn, "pop"))
#define mnPUSH(mn) !(strcmp(mn, "PUSH") && strcmp(mn, "push"))
#define mnCALL(mn) !(strcmp(mn, "CALL") && strcmp(mn, "call"))
#define mnRET(mn) !(strcmp(mn, "RET") && strcmp(mn, "ret"))
#define mnQUIT(mn) !(strcmp(mn, "QUIT") && strcmp(mn, "quit"))
#define mnDEBUG(mn) !(strcmp(mn, "DEBUG") && strcmp(mn, "debug"))
#define mnCMPNOT(mn) !(strcmp(mn, "CMPNOT") && strcmp(mn, "cmpnot"))
#define mnRETINT(mn) !(strcmp(mn, "RETINT") && strcmp(mn, "retint"))
#define mnSETINT(mn) !(strcmp(mn, "SETINT") && strcmp(mn, "setINT"))
#define macroRAWBYTE(mn) !(strcmp(mn, "RAWBYTE") && strcmp(mn, "rawbyte"))

#endif

