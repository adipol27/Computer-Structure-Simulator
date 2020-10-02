/*Authors   Adi Polak and Or Lopata.
Project – simulation
Description-This is a simulation to "fetch-decode-execute". It fetched the next command, decodes it, execute it and
promotes the pc counter by one. The program is finished when the  "halt" command is read.
argv inputs- "sim.exe memin.txt memout.txt regout.txt trace.txt count.txt"
*/
#define _CRT_SECURE_NO_DEPRECATE
#include "sim.h"



void add(int *rd, int *rs, int *rt)
{
	*rd = *rs + *rt;
}

void sub(int *rd, int *rs, int *rt)
{
	*rd = *rs - *rt;
}

void and(int *rd, int *rs, int *rt)
{
	*rd = (*rs) & (*rt);
}

void or (int *rd, int *rs, int *rt)
{
	*rd = (*rs) | (*rt);
}

void sll(int *rd, int *rs, int *rt)
{
	*rd = (*rs) << (*rt);
}

void sra(int *rd, int *rs, int *rt)
{
	*rd = (*rs) >> (*rt);
	*rd = (*rd) | (*rs & 0x80000000);
}

void beq(int *rd, int *rs, int *rt, int *pc, int *numOfLines)
{
	if ((*rs == *rt) && (*numOfLines >= *rd))
	{
		*pc = ((*rd) & 0xFFFF) - 1;
	}
}

void bgt(int *rd, int *rs, int *rt, int *pc, int *numOfLines)
{
	if ((*rs > *rt) && (*numOfLines >= ((*rd) & 0xFFFF)))
	{
		*pc = ((*rd) & 0xFFFF) - 1;
	}
}

void ble(int *rd, int *rs, int *rt, int *pc, int *numOfLines)
{
	if ((*rs <= *rt) && (*numOfLines >= ((*rd) & 0xFFFF)))
	{
		*pc = ((*rd) & 0xFFFF) - 1;
	}
}

void bne(int *rd, int *rs, int *rt, int *pc, int *numOfLines)
{
	if ((*rs != *rt) && (*numOfLines >= ((*rd) & 0xFFFF)))
	{
		*pc = ((*rd) & 0xFFFF) - 1;
	}
}

void jal(int *rd, int *pc, int *r15, int *numOfLines)
{
	*r15 = (*pc) + 1;
	*pc = ((*rd)&(0xFFFF));
	*pc = (*pc) - 1;
}

void lw(int *memPtr, int *regDes)
{
	int isNeg;
	*regDes = *memPtr;
	isNeg = (*memPtr) & 0x00008000;
	if (isNeg == 0x8000)
		*regDes = (*regDes) | 0xFFFF0000;
}

void sw(int *memPtr, int *regDes, int *numOfLines, int newLine)
{
	*memPtr = ((*regDes)&(0xFFFF));
	if (*numOfLines < newLine)
		*numOfLines = newLine;
}

void lhi(int *regDes, int *regSrc)
{
	*regDes = (*regDes)&(0xFFFF) + (((*regSrc)&(0xFFFF)) << 16);
}

void copyToMemout(FILE *memout, int numOfLines)
{
	for (int i = 0; i < numOfLines + 1; i++)
		fprintf(memout, "%04X\n", memArr[i]);
}

void fillRegisters(FILE *regout, int *regs)
{
	for (int i = 2; i < 16; i++)
		fprintf(regout, "%08X\n", regs[i]);
}

void memToArr(FILE *memin, int *numOfLines)
{
	int iArr = 0;
	while (!feof(memin))
	{
		fscanf_s(memin, "%x", &memArr[iArr]);
		iArr++;
	}
	*numOfLines = iArr - 1;
}

int excecute_command(struct command newComm, int *regs, int *pc, FILE *trace, int *numOfLines)
{
	int orgPC = *pc;						// we want to store the original  PC value because it might be changed
	
	newComm.rt = (newComm.binCommand) & (0x000F);						// to get the registers' numbers 
	newComm.rs = ((newComm.binCommand) & (0x00F0)) >> 4;
	newComm.rd = ((newComm.binCommand) & (0x0F00)) >> 8;
	newComm.opcode = ((newComm.binCommand) & (0xF000)) >> 12;		// to get the opcode of the command
	regs[1] = 0x00000000;											//regs[1] is not a real register and we zero it every iteration
	if (newComm.binCommand == 15)
		newComm.opcode = 15;
	if (newComm.rt == 0x0001 || newComm.rs == 0x0001 || newComm.rd == 0x0001)	// check if we use imm 
	{
		(*pc)++;
		regs[1] = regs[1] + memArr[*pc];
	}
	if ((regs[1] & 0x8000) == (0x8000))					// if the imm is negative number
		regs[1] = regs[1] | 0xFFFF0000;


	fprintf(trace, "%08X %04X", orgPC, (regs[1] & 0xFFFF));	// write the PC value and the 
	fprintf(trace, "%04X", (newComm.binCommand));

	for (int i = 0; i < 16; i++)			// write the values of the registers
		fprintf(trace, " %08X", regs[i]);
	fprintf(trace, "\n");
	if (newComm.opcode == 0 && (newComm.rd != 0))		// now we follow the command
		add(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt]);
	else if (newComm.opcode == 1 && (newComm.rd != 0))
		sub(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt]);
	else if (newComm.opcode == 2 && (newComm.rd != 0))
		and (&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt]);
	else if (newComm.opcode == 3 && (newComm.rd != 0))
		or (&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt]);
	else if (newComm.opcode == 4 && (newComm.rd != 0))
		sll(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt]);
	else if (newComm.opcode == 5 && (newComm.rd != 0))
		sra(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt]);
	else if (newComm.opcode == 7)
		beq(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt], pc, numOfLines);
	else if (newComm.opcode == 8)
		bgt(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt], pc, numOfLines);
	else if (newComm.opcode == 9)
		ble(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt], pc, numOfLines);
	else if (newComm.opcode == 10)
		bne(&regs[newComm.rd], &regs[newComm.rs], &regs[newComm.rt], pc, numOfLines);
	else if (newComm.opcode == 11)
		jal(&regs[newComm.rd], pc, &regs[15], numOfLines);
	else if (newComm.opcode == 12 && (newComm.rd != 0))
		lw(&memArr[regs[newComm.rs] + regs[newComm.rt]], &regs[newComm.rd]);
	else if (newComm.opcode == 13)
		sw(&memArr[regs[newComm.rs] + regs[newComm.rt]], &regs[newComm.rd], numOfLines, regs[newComm.rs] + regs[newComm.rt]);
	else if (newComm.opcode == 14 && (newComm.rd != 0))
		lhi(&regs[newComm.rd], &regs[newComm.rs]);
	else if (newComm.opcode == 15)
		return 0;

	return 1;
}

int scanArr(int *pc, int *regs, FILE *trace, int *numOfLines)
{
	struct command newComm;
	int flag = 1;
	int count = 0;
	*pc = 0x0;
	while (flag == 1)
	{
		newComm.binCommand = memArr[*pc];
		flag = excecute_command(newComm, regs, pc, trace, numOfLines);
		count++;
		(*pc)++;
		if (flag != 1)
			break;
	}
	return count;
}



int main(int argc, char *argv[])
{
	int linNum = 0;
	int *numOfLines;
	numOfLines = &linNum;
	FILE *memin, *memout, *regout, *trace, *countF;
	errno_t err;
	int count;
	int pcVal = 0;
	int *pc;
	pc = &pcVal;
	int regs[16] = { 0 };
	memin = fopen(argv[1], "r");
	if (memin == NULL)
	{
		printf("Couldn't open file\n");
		return 1;
	}
	memToArr(memin, numOfLines);
	fclose(memin);
	trace = fopen(argv[4], "w");
	if (trace == NULL)
	{
		printf("Couldn't open file\n");
		return 1;
	}
	*pc = 0;
	count = scanArr(pc, regs, trace, numOfLines);
	fclose(trace);
	memout = fopen(argv[2], "w");
	if (memout == NULL)
	{
		printf("Couldn't open file\n");
		return 1;
	}
	copyToMemout(memout, *numOfLines);
	fclose(memout);
	regout = fopen(argv[3], "w");
	if (regout == NULL)
	{
		printf("Couldn't open file\n");
		return 1;
	}
	fillRegisters(regout, regs);
	fclose(regout);
	countF = fopen(argv[5], "w");
	if (countF == NULL)
	{
		printf("Couldn't open file\n");
		return 1;
	}
	fprintf(countF, "%d", count);
	fclose(countF);
	return 0;
}
