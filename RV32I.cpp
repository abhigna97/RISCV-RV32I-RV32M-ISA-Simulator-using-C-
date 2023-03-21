#define DEBUG
// SILENT  		- 	prints PC and regs at end of all instructions
// VERBOSE 		- 	prints PC and regs at end of each instruction
// DEBUG 		- 	prints miscellaneous information
// DEBUGMEM 	- 	Prints Memory Contents
// STEPVERBOSE 	- 	Prints PC and regs at end of each instruction and needs user to press a key followed by "ENTER" to proceed to next instruction
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <bits/stdc++.h>
#include "declarations.h"					// Contains Memory, Constants and Instruction Fields Struct Declarations
#include "BaseExtendFunctions.h"			// Contains Definition of PC/Memory/Register Read/Write functions, RV32I,RV32M Functions.
#include "preload.h"						// File to Preload the registers and memory locations for Function Specific testing

/***************************************** EXECUTE FUNCTIONS SPECIFIC TO INSTRUCTION TYPE *****************************************/

uint32_t RtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t funct3temp,funct7temp;
	funct3temp = Fields->funct3;
	funct7temp = Fields->funct7;
    switch(funct7temp){
				case 0b0000000: switch (funct3temp){
									case 0b000: if(!ADD(Fields,RegisterFile)) 	return 0;		break;
									case 0b001: if(!SLL(Fields,RegisterFile)) 	return 0;		break;
									case 0b010: if(!SLT(Fields,RegisterFile)) 	return 0;		break;
									case 0b011: if(!SLTU(Fields,RegisterFile)) 	return 0;		break;
									case 0b100: if(!XOR(Fields,RegisterFile)) 	return 0;		break;
									case 0b101: if(!SRL(Fields,RegisterFile)) 	return 0;		break;
									case 0b110: if(!OR(Fields,RegisterFile)) 	return 0;		break;
									case 0b111: if(!AND(Fields,RegisterFile)) 	return 0;		break;
									default: cerr << "***ILLEGAL Funct3*** Supplied to R-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
								} break;
				case 0b0100000: switch (funct3temp){
									case 0b000: if(!SUB(Fields,RegisterFile)) return 0;		break;
									case 0b101: if(!SRA(Fields,RegisterFile)) return 0;		break;
									default : cerr << "***ILLEGAL Funct3*** Supplied to R-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
								} break;
				case 0b0000001: switch(funct3temp){
									case 0b000: if(!MUL(Fields,RegisterFile)) 		return 0;		break;
									case 0b001: if(!MULH(Fields,RegisterFile)) 		return 0;		break;
									case 0b010: if(!MULHSU(Fields,RegisterFile)) 	return 0;		break;
									case 0b011:	if(!MULHU(Fields,RegisterFile)) 	return 0;		break;
									case 0b100: if(!DIV(Fields,RegisterFile)) 		return 0;		break;
									case 0b101: if(!DIVU(Fields,RegisterFile)) 		return 0;		break;
									case 0b110: if(!REM(Fields,RegisterFile)) 		return 0;		break;
									case 0b111: if(!REMU(Fields,RegisterFile)) 		return 0;		break;
									default : cerr << "***ILLEGAL Funct3*** Supplied to R-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
								} break;
				default : cerr << "***ILLEGAL Funct7*** Supplied to R-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t ItypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t opcodetemp,funct3temp,imm11_5temp;
	opcodetemp = Fields->opcode;
	funct3temp = Fields->funct3;
	imm11_5temp=Fields->imm_I11_5;
	switch(opcodetemp){
    case 0b0000011:	switch(funct3temp){
						case 0b000: if(!LB(Fields,RegisterFile,Memory)) 	return 0;		break;
						case 0b001: if(!LH(Fields,RegisterFile,Memory)) 	return 0;		break;
						case 0b010: if(!LW(Fields,RegisterFile,Memory)) 	return 0;		break;
						case 0b100: if(!LBU(Fields,RegisterFile,Memory)) 	return 0;		break;
						case 0b101: if(!LHU(Fields,RegisterFile,Memory)) 	return 0;		break;
						default : cerr << "***ILLEGAL Funct3*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
					} break;
	case 0b0010011:	switch(funct3temp){
						case 0b000: if(!ADDI(Fields,RegisterFile)) 	return 0;		break;
						case 0b001: if(!SLLI(Fields,RegisterFile)) 	return 0;		break;
						case 0b010: if(!SLTI(Fields,RegisterFile)) 	return 0;		break;
						case 0b011: if(!SLTIU(Fields,RegisterFile)) return 0;		break;
						case 0b100: if(!XORI(Fields,RegisterFile)) 	return 0;		break;
						case 0b101: switch(imm11_5temp){
										case 0b0000000:	if(!SRLI(Fields,RegisterFile)) return 0;		break;
										case 0b0100000: if(!SRAI(Fields,RegisterFile)) return 0;		break;
										default: cerr << "***ILLEGAL Imm[11:5]*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
									} break;
						case 0b110: if(!ORI(Fields,RegisterFile)) 	return 0;		break;
						case 0b111: if(!ANDI(Fields,RegisterFile)) 	return 0;		break;
						default : cerr << "***ILLEGAL Funct3*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
					} break;
	case 0b1100111: if(!JALR(Fields,RegisterFile)) return 0;		break;
	default : cerr << "***ILLEGAL Opcode*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t StypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t funct3temp;
	funct3temp = Fields->funct3;
	switch(funct3temp){
				case 0b000: if(!SB(Fields,RegisterFile,Memory)) return 0;		break;		
				case 0b001: if(!SH(Fields,RegisterFile,Memory)) return 0;		break;
				case 0b010: if(!SW(Fields,RegisterFile,Memory)) return 0;		break;
				default : cerr << "***ILLEGAL Funct3*** Supplied to S-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t BtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t funct3temp;
	funct3temp = Fields->funct3;
	switch(funct3temp){
				case 0b000: if(!BEQ(Fields,RegisterFile)) 	return 0;		break;
				case 0b001: if(!BNE(Fields,RegisterFile)) 	return 0;		break;
				case 0b100: if(!BLT(Fields,RegisterFile)) 	return 0;		break;
				case 0b101: if(!BGE(Fields,RegisterFile)) 	return 0;		break;
				case 0b110: if(!BLTU(Fields,RegisterFile)) 	return 0;		break;
				case 0b111: if(!BGEU(Fields,RegisterFile)) 	return 0;		break;
				default : cerr << "***ILLEGAL Funct3*** Supplied to B-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t UtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t opcodetemp;
	opcodetemp = Fields->opcode;
    switch(opcodetemp){
				case 0b0110111: if(!LUI(Fields,RegisterFile)) 	return 0;		break;
				case 0b0010111: if(!AUIPC(Fields,RegisterFile)) return 0;		break;
				default : cerr << "***ILLEGAL Funct3*** Supplied to U-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t JtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	if(!JAL(Fields,RegisterFile)) return 0;
	else return 1;
}

/***************************************** DECODE FUNCTIONS SPECIFIC TO INSTRUCTION TYPE *****************************************/

uint32_t RtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp;								// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get rs2
    Fields->rs2 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs2
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get funct7
    Fields->funct7 	= instrtemp & 0X0000007F; 		// masking instrtemp with a mask value to get value of funct7
	#ifdef DEBUG
		printf("***R-TYPE:0x%08x****\nfunct7\t=0x%02x\n,rs2\t=0x%02x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->funct7,Fields->rs2,Fields->rs1,Fields->funct3,Fields->rd,Fields->opcode);
	#endif
	if(RtypeExecute(Fields, instruction)) return 1;
	else return 0;
}

uint32_t ItypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp; 							// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get shamt/imm[4:0]
    Fields->shamt 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of shamt/imm[4:0]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get imm[11:5]
    Fields->imm_I11_5= instrtemp & 0x0000007F; 		// masking instrtemp with a mask value to get value of imm[11:5]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get imm[11:0]
    Fields->imm_I11_0 	= instrtemp & 0x00000FFF; 	// masking instrtemp with a mask value to get value of imm[11:0]
    #ifdef DEBUG
		printf("***I-TYPE:0x%08x****\nimm[11:0]\t=0x%03x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_I11_0,Fields->rs1,Fields->funct3,Fields->rd,Fields->opcode);
	#endif
	if(ItypeExecute(Fields, instruction)) return 1;
	else return 0;
}

uint32_t StypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,immtemp; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get imm[4:0]
	Fields->imm_S4_0= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of imm[4:0]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get rs2
    Fields->rs2 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs2
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get imm[11:5]
    Fields->imm_S11_5 = instrtemp & 0X0000007F; 	// masking instrtemp with a mask value to get value of imm[11:5]
	immtemp 		= Fields->imm_S11_5 << 5;		// combining parts of immediate field to form a single immediate field
	immtemp			= immtemp | Fields->imm_S4_0;
	Fields->imm_S11_0 = immtemp;
	#ifdef DEBUG
		printf("***S-TYPE:0x%08x****\nimm[11:0]\t=0x%02x\n,imm[11:5]\t=0x%02x\n,rs2\t=0x%02x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,imm[4:0]\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_S11_0,Fields->imm_S11_5,Fields->rs2,Fields->rs1,Fields->funct3,Fields->imm_S4_0,Fields->opcode);
	#endif
	if(StypeExecute(Fields, instruction)) return 1;
	else return 0;
}

uint32_t BtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,immtemp; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get imm[11]
	Fields->imm_B11 = instrtemp & 0x00000001; 		// masking instrtemp with mask value to get value of imm[11]
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 8; 				// shifting instrtemp by 6 positions to get imm[4:1]
	Fields->imm_B4_1= instrtemp & 0x0000000F; 		// masking instrtemp with mask value to get value of imm[4:1]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get rs2
    Fields->rs2 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs2
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get imm[10:5]
    Fields->imm_B10_5 = instrtemp & 0X0000003F; 	// masking instrtemp with a mask value to get value of imm[10:5]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 31; 				// shifting instrtemp by 25 positions to get imm[11:5]
    Fields->imm_B12 = instrtemp & 0X00000001; 		// masking instrtemp with a mask value to get value of imm[11:5]
	immtemp 		= Fields->imm_B12 << 1;			// combining parts of immediate field to form a single immediate field
	immtemp			= immtemp | Fields->imm_B11;
	immtemp			= immtemp << 6;
	immtemp			= immtemp | Fields->imm_B10_5;
	immtemp			= immtemp << 4;
	immtemp			= immtemp | Fields->imm_B4_1;
	Fields->imm_B12_1 = immtemp;
	#ifdef DEBUG
		printf("***B-TYPE:0x%08x****\nimm[12:1]\t=%0x\n,imm[12]\t=%0x\n,imm[10:5]\t=%0x\n,rs2\t=%0x\n,rs1\t=%0x\n,funct3\t=%0x\n,imm[4:1]\t=%0x\n,imm[11]\t=%0x\n,opcode\t=%0x\n",instruction,Fields->imm_B12_1,Fields->imm_B12,Fields->imm_B10_5,Fields->rs2,Fields->rs1,Fields->funct3,Fields->imm_B4_1,Fields->imm_B11,Fields->opcode);
	#endif
	if(BtypeExecute(Fields, instruction)) return 1;
	else return 0;
}

uint32_t UtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp; 							// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get imm[31:12]
    Fields->imm_U31_12 	= instrtemp & 0x000FFFFF; 	// masking instrtemp with a mask value to get value of imm[31:12]
	#ifdef DEBUG
		printf("***U-TYPE:0x%08x****\nimm[31:12]\t=0x%05x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_U31_12,Fields->rd,Fields->opcode);
	#endif
	if(UtypeExecute(Fields, instruction)) return 1;
	else return 0;
}

uint32_t JtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,immtemp; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 12; 				// shifting instrtemp by 6 positions to get imm[19:12]
	Fields->imm_J19_12= instrtemp & 0x000000FF; 	// masking instrtemp with mask value to get value of imm[19:12]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 20; 				// shifting instrtemp by 12 positions to get imm[11]
    Fields->imm_J11 = instrtemp & 0x00000001; 		// masking instrtemp with a mask value to get value of imm[11]
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 21; 				// shifting instrtemp by 15 positions to get imm[10:1]
    Fields->imm_J10_1= instrtemp & 0x000003FF; 		// masking instrtemp with a mask value to get value of imm[10:1]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 31; 				// shifting instrtemp by 20 positions to get imm[20]
    Fields->imm_J20	= instrtemp & 0x00000001; 		// masking instrtemp with a mask value to get value of imm[20]
	immtemp 		= Fields->imm_J20 << 8;			// combining parts of immediate field to form a single immediate field
	immtemp			= immtemp | Fields->imm_J19_12;
	immtemp			= immtemp << 1;
	immtemp			= immtemp | Fields->imm_J11;
	immtemp			= immtemp << 10;
	immtemp			= immtemp | Fields->imm_J10_1;
	Fields->imm_J20_1 = immtemp;
	#ifdef DEBUG
		printf("***J-TYPE:0x%08x****\nimm[20]\t=0x%01x\n,imm[20]\t=0x%01x\n,imm[10:1]\t=0x%03x\n,imm[11]\t=0x%01x\n,imm[19:12]\t=0x%02x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_J20_1,Fields->imm_J20,Fields->imm_J10_1,Fields->imm_J11,Fields->imm_J19_12,Fields->rd,Fields->opcode);
	#endif
	if(JtypeExecute(Fields, instruction)) return 1;
	else return 0;
}

/***************************************** MAIN FUNCTION FOR RISCV SIMULATOR *****************************************/

int main(int argc, char *argv[]) {
 
	string memoryimage= "program.mem";		// Default memory image file
	uint32_t max_pc;						// To track the Maximum value of PC supplied from mem file
	char choice;
	uint32_t count=0;

    if (argc > 4) {
        cerr << "***CHECK ARGUMENTS*** Incorrect Number of Arguments Provided. Should be Used as follows: \n" << argv[0] << " <pc> <sp> <memoryimage>" << endl;
        return EXIT_FAILURE;
    } else {																// Parse PC and SP as 32-bit hexadecimal values, and Memory Image File as a String
		if(argc>1){								
			stringstream ss_pc(argv[1]);
			ss_pc >> hex >> pc;	
		}
		if(argc>2){
			stringstream ss_sp(argv[2]);
			ss_sp >> hex >> sp;
			set_reg(2,sp);
		} 	
		if(argc>3){
			memoryimage = string(argv[3]);
		}	
	}
	
	#ifdef DEBUG
    cout << "PC = 0x" << setfill('0') << setw(8) << hex << pc << endl;		// printing the parsed arguments in DEBUG Mode
    cout << "SP = 0x" << setfill('0') << setw(8) << hex << sp << endl;
    cout << "Memory image filename = " << memoryimage << endl;
	#endif
	
	ifstream memory_file(memoryimage);										// Loading the Memory Image into Memory Map
	uint32_t address,data;
	if (memory_file.is_open()) {
        string line;
        while (getline(memory_file, line)) {
            stringstream ss(line);
            ss >> hex >> address;
			if(address%4 != 0){
				cerr << "***ILLEGAL PC PROVIDED IN MEMFILE*** Wrong Address detected in the Memory Image. Instruction Address Can only be a multiple of 4. Stopped at address 0x" << hex << address << endl;
				return 0;
			}
			if(address >= MEM_SIZE){
				cerr << "***MEMORY ADDRESS OUT OF BOUNDS*** Instruction Memory is limited to 64KB Wrong Address detected in the Memory Image. Stopped at address 0x" << hex << address << endl;
				return 0;
			}									
            ss.ignore(2, ' '); 												// Ignore the colon and space between address and data
            ss >> hex >> data;
            Memory[address] = data & 0xFF;
			Memory[address + 1] = (data >> 8)  & 0xFF;
			Memory[address + 2] = (data >> 16) & 0xFF;
			Memory[address + 3] = (data >> 24) & 0xFF;
			max_pc = address + 4;
		}
		#ifdef DEBUG
			printf("max_pc=0x%08x\n",max_pc);
		#endif	
        memory_file.close();
    } else {
        cerr << "***FILE ERROR*** Failed to open memory image file." << endl;
        return 0;
    }
	
	#ifdef DEBUG															// To preload Memory/Registers for Instruction Debug Purposes
		PREload();
	#endif
	
	if(pc > (Memory.rbegin() -> first)){
		cerr <<"***WRONG INPUTS*** PC value is not within mem file range.Please enter a valid PC value in the First argument you supply" << endl;
		return 0;
	} else {
		for (pc ; pc < max_pc; pc = pc + 4) {
			uint32_t currentPC = pc;	
			if(currentPC % 4 == 0) {
				uint32_t currentINSTR = Memory[currentPC] | (Memory[currentPC + 1] << 8) | (Memory[currentPC + 2] << 16) | (Memory[currentPC + 3] << 24);
				if(currentINSTR == 0x00000000){
					cerr << "***TRAP*** Instruction at address:0x" << hex << currentPC << " is all 0's. Terminating the Simulation" << endl;
					return 0;
				} else {
					count ++;
					uint32_t opcode = currentINSTR & 0x7F;
						#ifdef VERBOSE
							cout << "******************************** Instruction: 0x" << hex << currentINSTR << " ***********************************\n" << endl;
						#endif
						#ifdef STEPVERBOSE
							cout << "\n****Executing Instruction #" << count <<"****\n\nChoose to print : Instruction(I/i),Registers(R/r),Memory(M/m), All (A/a), Only Instruction&Registers (B/b),\n Only Instruction&Memory (C/c),Only Registers&Memory (D/d),No(Any Other Key) ****\n" ;
							cin >> choice;
							if(choice == 'I' || choice == 'i' || choice == 'A' || choice == 'a' || choice == 'B' || choice == 'b' || choice == 'C' || choice == 'c' ){
								cout << "******************************** Instruction: 0x" << hex << currentINSTR << " ***********************************\n" << endl;
							}
						#endif
						switch(opcode){
							case 0b0110111: if(!UtypeDecode(&Fields,currentINSTR)) return 0;	break;	// For LUI
							case 0b0010111: if(!UtypeDecode(&Fields,currentINSTR)) return 0;	break;	// For AUIPC
							case 0b1101111: if(!JtypeDecode(&Fields,currentINSTR)) return 0;	break;	// For JAL
							case 0b1100111: if(!ItypeDecode(&Fields,currentINSTR)) return 0;	break;	// For JALR
							case 0b1100011: if(!BtypeDecode(&Fields,currentINSTR)) return 0;	break;	// For BEQ,BNE,BLT,BGE,BLTU,BGEU
							case 0b0000011: if(!ItypeDecode(&Fields,currentINSTR)) return 0;	break;	// For LB,LH,LW,LBU,LHU
							case 0b0100011: if(!StypeDecode(&Fields,currentINSTR)) return 0;	break;	// For SB,SH,SW
							case 0b0010011: if(!ItypeDecode(&Fields,currentINSTR)) return 0;	break;	// For ADDI,SLTI,SLTIU,ANDI,ORI,XORI,SLLI,SRLI,SRAI
							case 0b0110011: if(!RtypeDecode(&Fields,currentINSTR)) return 0;	break;	// For ADD,SLT,SLTU,AND,OR,XOR,SLL,SRL,SUB,SRA
							//case 0b0001111: break; 							 						// For FENCE
							//case 0b1110011: if(!ItypeDecode(&Fields,currentINSTR)) return 0;	break;	// For ECALL,EBREAK
							default : 		cerr << "***ILLEGAL OPCODE*** Detected at address(hex): "<< hex << currentPC << "\tInstruction:"<< setfill('0') << setw(8) << hex << currentINSTR <<"\topcode(binary): " << bitset<7>(opcode) <<"\n"; 
											return 0; break;
						}
						#ifdef VERBOSE
							print_regs();
							cout << "*********************************** YOU DESERVE EXTRA CREDIT ***********************************\n" << endl;
						#endif
						#ifdef STEPVERBOSE
							if(choice == 'R' || choice == 'r'|| choice == 'B' || choice == 'b'){
								print_regs();
							} else if (choice == 'M' || choice == 'm'|| choice == 'C' || choice == 'c'){
								print_memory();
							} else if (choice == 'A' || choice == 'a' || choice == 'D' || choice == 'd'){
								print_regs();
								print_memory();
							} else {
								cout << "\n\nThanks...Proceeding to next instruction execution....\n\n" << endl;
							};
						#endif
				}
			} else {
				cerr << "***TRAP*** MISALIGNED INSTRUCTION ADDRESS is fetched at PC(Hex) :0x" << hex << pc << endl;
					return 0;
			}
		}
	}
	#ifdef SILENT
		cout << "************************************ SIMULATION SUMMARY ***************************************\n" << endl;
		print_regs();
		cout << "*********************************** YOU DESERVE EXTRA CREDIT ***********************************\n" << endl;
	#endif
	#ifdef DEBUGMEM
    print_memory();
	#endif
    return EXIT_SUCCESS;
	
}
// commands to use: g++ -o rv RV32I.cpp ; ./rv PC SP MEMFILE