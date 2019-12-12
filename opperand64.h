#ifndef OPPERAND
#define OPPERAND

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


#define CMP_RN 0x320
#define CMP_RR 0x321

#define PUSH 0x323
#define PULL 0x324

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

#define DSP_R 0x3FF

#define LAB 0x000
#define JMP 0x100
#define JZ 0x101
#define JB 0x102
#define JSE 0x103

#define FUNC 0x000
#define CALL 0x110
#define RET 0x111

#endif

