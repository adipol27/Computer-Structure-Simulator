#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int memArr[65536] = { 0 };
struct command {
	int binCommand;	// The original command from memory (4 bytes)
	int opcode;
	int rs;
	int rt;
	int rd;
};


void add(int *rd, int *rs, int *rt);

void sub(int *rd, int *rs, int *rt);

void and(int *rd, int *rs, int *rt);

void or (int *rd, int *rs, int *rt);

void sll(int *rd, int *rs, int *rt);

void sra(int *rd, int *rs, int *rt);

void beq(int *rd, int *rs, int *rt, int *pc, int *numOfLines);

void bgt(int *rd, int *rs, int *rt, int *pc, int *numOfLines);

void ble(int *rd, int *rs, int *rt, int *pc, int *numOfLines);

void bne(int *rd, int *rs, int *rt, int *pc, int *numOfLines);

void jal(int *rd, int *pc, int *r15, int *numOfLines);


void lw(int *memPtr, int *regDes);


void sw(int *memPtr, int *regDes, int *numOfLines, int newLine);

void lhi(int *regDes, int *regSrc);

void copyToMemout(FILE *memout, int numOfLines);

void fillRegisters(FILE *regout, int *regs);


void memToArr(FILE *memin, int *numOfLines);

int excecute_command(struct command newComm, int *regs, int *pc, FILE *trace, int *numOfLines);

int scanArr(int *pc, int *regs, FILE *trace, int *numOfLines);

