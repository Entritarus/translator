#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define LINE_LEN 120
#define LINEBUF_LEN 121 

// string length is always 1 symbol longer 
// than the actual string length due to \0 symbol 

#define CMD_CNT 34
#define CMD_LEN 5

#define N_GPR 8
#define MAX_GPR 7
#define N_ROM 256
#define MAX_ROM 255
#define N_RAM 16
#define MAX_RAM 15

int translate(char *buf);

char ASMcmd[CMD_CNT][CMD_LEN] = {
	"ADD",
	"ADC",
	"INC",
	"DEC",
	"NEG",
	"SUB",
	"SBC",
	"MOV",
	"AND",
	"OR",
	"XOR",
	"NOT",
	"LSL",
	"LSR",
	"ROL",
	"ROR",
	"CMP",
	"CPC",
	"LDI",
	"LDR",
	"STR",
	"JMP",
	"BRCC",
	"BRCS",
	"BRZC",
	"BRZS",
	"BRNC",
	"BRNS",
	"BRVC",
	"BRVS",
	"PUSH",
	"POP",
	"CALL",
	"RET"
};

typedef enum {
	ADD = 0,
	ADC, // 1
	INC, // 2
	DEC, // 3
	NEG, // 4
	SUB, // 5
	SBC, // 6
	MOV, // 7
	AND, // 8
	OR, // 9
	XOR, // 10
	NOT, // 11
	LSL, // 12
	LSR, // 13
	ROL, // 14
	ROR, // 15
	CMP, // 16
	CPC, // 17
	LDI, // 18
	LDR, // 19
	STR, // 20
	JMP, // 21
	BRCC, // 22
	BRCS, // 23
	BRZC, // 24
	BRZS, // 25
	BRNC, // 26
	BRNS, // 27
	BRVC, // 28
	BRVS, // 29
	PUSH, // 30
	POP, // 31
	CALL, // 32
	RET //33
} cmd_t;

int main(int argc, int **argv) {
	
	FILE *fp_src = fopen((char*)argv[1], "r"); // open src file
	if (!fp_src) {
		printf("Error: Failed to open file\n");
		return EXIT_FAILURE;
	}
	
	char *outName = (char*) calloc(strlen((char*)argv[1])+1, sizeof(char)); // allocate memory for a new name
	char extension[] = "hex"; // output file extension will be hex
	strcpy(outName, (char*)argv[1]); // copy from input arguments, because arguments must not be changed
	for (int i = strlen((char*)argv[1]) - 3; i < strlen((char*)argv[1]); i++) { //overwrite extension
		outName[i] = extension[i-(strlen((char*)argv[1]) - 3)];
	}
	
	FILE *hex = fopen(outName, "w");
	fprintf(hex,"v2.0 raw\n"); // write header
	
	char linebuf[LINEBUF_LEN] = {}; // line storage
	linebuf[LINE_LEN] = -1; // this is used to detect lines that are too long
	unsigned int opcode = 0x0;
	unsigned int lineNum = 0;
	while(!feof(fp_src)) {
		if(fgets(linebuf, LINE_LEN+1, fp_src) == NULL) {// get line
			printf("Error: File read failure\n"); 
			break;
		}
		if(strlen(linebuf) == 1) continue; // skip empty lines with only \n symbol
		if(linebuf[LINE_LEN] == -1) // line length detection
			opcode = translate(linebuf); // decode opcode from line
		else 
		{
			printf("Error: Maximum line length is 120 characters\n");
			break;
		}
		if (lineNum < N_ROM) // maximum instruction count is 256 because JMP and branch instructions can only go to addresses in range 0..255
			lineNum++; 
		else {
			printf("Error: Max instruction count exceeded (N_ROM)\n");
		}
		if(opcode != -1){
			//write to file
			fprintf(hex, "%04x ", opcode);
			printf("Translate success! opcode = 0x%04x\n", opcode);
		} else {
			//exit with error
			printf("Translate error! Line: %d\n", lineNum);
			break;
		}
	}
	char haltCmd[10] = "JMP ";
	char lineNumStr[4] = {};
	sprintf(lineNumStr, "%d", lineNum);
	strcat(haltCmd, lineNumStr);
	// because in the code the halt command is the lineNum + 1 st command,
	// but in the memory it is less by one, the +1 and -1 vanish
	opcode = translate(haltCmd);
	fprintf(hex, "%04x ", opcode);
	
	fclose(fp_src);
	fclose(hex);
	free(outName);
	return 0;
}




int translate(char *buf) {
	// find command in the line and in the list
	cmd_t cmd;
	char *comloc;
	for (cmd = 0; cmd < CMD_CNT; cmd++) {
		comloc = strstr(buf, ASMcmd[cmd]);
		if (comloc != NULL) break;
	}
	//printf("cmd = %d\n", cmd);
	
	if (comloc == NULL){
		printf("Error: Unknown command\n");
		return -1;
	}
	// check if OR instruction is actually ROR or XOR
	if (buf != comloc) { 
		// buf points at the first character of the buffer
		// comloc points at the first character of the found instruction
		// they are different if 
		// 1. there are spaces before instruction
		// 2. OR is actually ROR or XOR because strstr may find OR in ROR and XOR
		if (*(comloc-1) == 'R') cmd = ROR;
		if (*(comloc-1) == 'X') cmd = XOR;
	}
	printf("Command found: %s\n", ASMcmd[cmd]);
	unsigned int opcode = 0;
	
	switch(cmd) {
		case ADD:
		case ADC:
		case INC:// 1 operand
		case DEC:// 1 operand
		case NEG:// 1 operand
		case SUB:
		case SBC:
		case MOV:
		case AND:
		case OR:
		case XOR:
		case NOT:// 1 operand
		case LSL:// 1 operand
		case LSR:// 1 operand
		case ROL:// 1 operand
		case ROR:// 1 operand
		case CMP:
		case CPC: { 
			opcode |= (cmd == CMP || cmd == CPC) << 12;
			opcode |= (cmd == CMP ? SUB : (cmd == CPC ? SBC : cmd)) << 4;
			// ALU instructions and CMP, CPC are really similar, so they are joined
			int Rd = 0, Rs = 0;
			char command[5];
			int count = sscanf(buf, "%s R%d, R%d", command, &Rd, &Rs); // get instruction and input parameters
			if (count < (cmd == INC || cmd == DEC || cmd == NEG || cmd == NOT || cmd == LSL || cmd == LSR || cmd == ROL || cmd == ROR ? 2 : 3)) { 
				// instructions require 1 or 2 operands
				printf("Error: %s: Not enough input parameters\n", command);
				return -1;
			}
				
			if (Rd >= 0 && Rd < N_GPR) { // check range for Rd
				opcode |= Rd << 8;
			} else {
				printf("Error: Rd must be in range 0..%d\n", MAX_GPR);
				return -1;
			}
			
			if (Rs >= 0 && Rs < N_GPR) { // check range for Rs. For single operand instructions Rs is always 0
				opcode |= Rs << 0;
			} else {
				printf("Error: Rs must be in range 0..%d\n", MAX_GPR);
				return -1;
			}
		}
		break;
		
		case LDI:
		case LDR:
		case STR: {
			opcode |= (cmd-16) << 12; // offset needed because the first 17 instructions are ALU instructions
			int Rd = 0, A = 0;
			char command[5];
			int count = sscanf(buf, "%s R%d, %d", command, &Rd, &A); // get instruction and input parameters
			if (count < 3) {
				printf("Error: %s: Not enough input parameters\n", command);
				return -1;
			}
			if (Rd >= 0 && Rd < N_GPR) { // check range for Rd
				opcode |= Rd << 8;
			} else {
				printf("Error: Rd must be in range 0..%d\n", MAX_GPR);
				return -1;
			}
			if (cmd == LDI) { // check ranges for A
				if (A >= 0 && A < 256) // this is due to the size of machine code instruction size
					opcode |= A;
				else {
					printf("Error: A must be in range 0..%d\n", 255); 
					return -1;
				}
			} else {
				if (A >= 0 && A < N_RAM)
					opcode |= A;
				else {
					printf("Error: A must be in range 0..%d\n", MAX_RAM);
					return -1;
				}
			}
		}
			
		break;
		
		case JMP:
		case BRCC:
		case BRCS:
		case BRZC:
		case BRZS:
		case BRNC:
		case BRNS:
		case BRVC:
		case BRVS:
		case CALL:	{
			if (cmd == JMP) {
				opcode |= 0x5 << 12;
			} else if (cmd >= BRCC && cmd <= BRVS) {
				int condition = cmd - BRCC; // branch instructions start from BRCC in the enum
				opcode |= 0x6 << 12 | (condition & 0x01) << (2+8) | (condition & 0x6) << (8-1);
				// construct opcode by formula
				// 0110 0vff aaaa aaaa
			} else opcode |= 0x9 << 12; // if cmd == CALL
			int A = 0;
			char command[5];
			int count = sscanf(buf, "%s %d", command, &A); // get instruction and one input parameter
			if (count < 2) {
				printf("Error: %s: Not enough input parameters\n", command);
				return -1;
			}
			if (A >= 0 && A < N_ROM)
				opcode |= A;
			else {
				printf("Error: A must be in range 0..%d\n", MAX_ROM);
				return -1;
			}
		}
		break;
		
		case PUSH:
		case POP:
			opcode |= ((cmd == PUSH) ? 0x7 : 0x8) << 12;
			int Rd = 0;
			char command[5];
			int count = sscanf(buf, "%s R%d", command, &Rd); // get instruction and input parameters
			if (count < 2) {
				printf("Error: %s: Not enough input parameters\n", command);
				return -1;
			}
			if (Rd >= 0 && Rd < N_GPR) { // check range for Rd
				opcode |= Rd << 8;
			} else {
				printf("Error: Rd must be in range 0..%d\n", MAX_GPR);
				return -1;
			}
		break;
		case RET:
			opcode |= 0xA << 12;
		break;
	}
	return opcode;
}
