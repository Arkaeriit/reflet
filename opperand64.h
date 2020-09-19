#ifndef OPPERAND
#define OPPERAND

//Basic opperands
#define NIL 0x000
#define QUIT 0x001

#define MOV_RN 0x300
#define MOV_RR 0x301

#define ADD_RN 0x302
#define ADD_RR 0x303
#define ADD_RRR 0x304

#define SUB_RN 0x306
#define SUB_RR 0x307
#define SUB_RRR 0x308

#define TIM_RN 0x30A
#define TIM_RR 0x30B
#define TIM_RRR 0x30C

#define DIV_RN 0x30E
#define DIV_RR 0x30F
#define DIV_RRR 0x310

#define MOD_RN 0x312
#define MOD_RR 0x313
#define MOD_RRR 0x314

#define AND_RN 0x316
#define AND_RR 0x317
#define AND_RRR 0x318

#define OR_RN 0x31A
#define OR_RR 0x21B
#define OR_RRR 0x31C

#define XOR_RN 0x31E
#define XOR_RR 0x31F
#define XOR_RRR 0x320

#define NOT_RN 0x322
#define NOT_RR 0x323
#define NOT_R 0x324


#define CMP_RN 0x330
#define CMP_RR 0x331

#define PUSH 0x333
#define POP 0x334

//Memory management
#define FREE 0x325
#define ASK_RR 0x326
#define ASK_RN 0x327
#define ASK_BYTE_RR 0x328
#define ASK_BYTE_RN 0x329
#define STR 0x32A
#define LDR 0x32B
#define STR_BYTE 0x32C
#define LDR_BYTE 0x32D
#define DATA 0x32E /*Note : not used*/
#define LDR_DATA 0x32E

//ABTFS partitions
#define A_IMG_OPEN 0x3D0
#define A_IMG_CLOSE 0x3D1
#define A_IMG_CREATE 0x3D2
#define A_NFILES 0x3D3
#define A_FSIZE 0x3D4
#define A_READ 0x3D5
#define A_WRITE 0x3D6

//File and Cstrings
#define OPEN 0x3E0
#define CLOSE 0x3E1
#define STDIN 0x3E2
#define STDOUT 0x3E3
#define STDERR 0x3E4
#define GCHAR 0x3E5
#define PCHAR 0x3E6
#define WRITE 0x3E7
#define READ 0x3E8
#define READ_LINE 0x3E9
#define STRLEN 0x3EA
#define STRCMP 0x3EB
#define EOFCMP 0x3EC
#define STN 0x3ED
#define NTS 0x3EE

#define SPRINT 0x3FE
#define DSP_R 0x3FF

//Flow control
#define LAB 0x000
#define JMP 0x100
#define JZ 0x101
#define JNZ 0x102
#define JB 0x103
#define JBE 0x104
#define JS 0x105
#define JSE 0x106

#define FUNC 0x000
#define CALL 0x110
#define RET 0x111

#endif

