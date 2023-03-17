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
#include "declarations.h"
#include "functions.h"
#include "preload.h"
#define DEBUGMEM				
// SILENT  	- 	prints PC and regs at end of all instructions
// VERBOSE 	- 	prints PC and regs at end of each instruction
// DEBUG 	- 	prints miscellaneous information
// DEBUGMEM - 	Prints Memory Contents
uint32_t RtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t funct3temp,funct7temp;
	funct3temp = Fields->funct3;
	funct7temp = Fields->funct7;
    switch(funct7temp){
				case 0b0000000: switch (funct3temp){
									case 0b000: ADD(Fields,RegisterFile);	break;
									case 0b001: SLL(Fields,RegisterFile);	break;
									case 0b010: SLT(Fields,RegisterFile);	break;
									case 0b011: SLTU(Fields,RegisterFile);	break;
									case 0b100: XOR(Fields,RegisterFile);	break;
									case 0b101: SRL(Fields,RegisterFile);	break;
									case 0b110: OR(Fields,RegisterFile);	break;
									case 0b111: AND(Fields,RegisterFile);	break;
									default: cerr << "***ILLEGAL Funct3*** Supplied to R-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
								} break;
				case 0b0100000: switch (funct3temp){
									case 0b000: SUB(Fields,RegisterFile);	break;
									case 0b101: SRA(Fields,RegisterFile);	break;
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
						case 0b000: LB(Fields,RegisterFile,Memory); 	break;
						case 0b001: LH(Fields,RegisterFile,Memory); 	break;
						case 0b010: LW(Fields,RegisterFile,Memory); 	break;
						case 0b100: LBU(Fields,RegisterFile,Memory); 	break;
						case 0b101: LHU(Fields,RegisterFile,Memory); 	break;
						default : cerr << "***ILLEGAL Funct3*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
					} break;
	case 0b0010011:	switch(funct3temp){
						case 0b000: ADDI(Fields,RegisterFile); 		break;
						case 0b001: SLLI(Fields,RegisterFile); 		break;
						case 0b010: SLTI(Fields,RegisterFile); 		break;
						case 0b011: SLTIU(Fields,RegisterFile); 	break;
						case 0b100: XORI(Fields,RegisterFile); 		break;
						case 0b101: switch(imm11_5temp){
										case 0b0000000:	SRLI(Fields,RegisterFile); 	break;
										case 0b0100000: SRAI(Fields,RegisterFile); 	break;
										default: cerr << "***ILLEGAL Imm[11:5]*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
									} break;
						case 0b110: ORI(Fields,RegisterFile); 	break;
						case 0b111: ANDI(Fields,RegisterFile); 	break;
						default : cerr << "***ILLEGAL Funct3*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
					} break;
	default : cerr << "***ILLEGAL Opcode*** Supplied to I-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t StypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t funct3temp;
	switch(funct3temp){
				case 0b000: SB(Fields,RegisterFile,Memory); 	break;		
				case 0b001: SH(Fields,RegisterFile,Memory); 	break;
				case 0b010: SW(Fields,RegisterFile,Memory); 	break;
				default : cerr << "***ILLEGAL Funct3*** Supplied to S-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t BtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t funct3temp;
	funct3temp = Fields->funct3;
	switch(funct3temp){
				case 0b000: BEQ(Fields,RegisterFile); 	break;
				case 0b001: BNE(Fields,RegisterFile); 	break;
				case 0b100: BLT(Fields,RegisterFile); 	break;
				case 0b101: BGE(Fields,RegisterFile); 	break;
				case 0b110: BLTU(Fields,RegisterFile); 	break;
				case 0b111: BGEU(Fields,RegisterFile); 	break;
				default : cerr << "***ILLEGAL Funct3*** Supplied to B-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t UtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	uint32_t opcodetemp;
	opcodetemp = Fields->opcode;
    switch(opcodetemp){
				case 0b0110111: LUI(Fields,RegisterFile); 		break;
				case 0b0010111: AUIPC(Fields,RegisterFile); 	break;
				default : cerr << "***ILLEGAL Funct3*** Supplied to U-TYPE by Instruction(hex): " << hex << instruction << endl; return 0; break;
	}
	return 1;
}

uint32_t JtypeExecute(struct InstrFields *Fields, uint32_t instruction){
	JAL(Fields,RegisterFile);
	return 1;
}

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
	uint32_t instrtemp; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get imm[31:12]
    Fields->imm_U31_12 	= instrtemp & 0x000FFFFF; 		// masking instrtemp with a mask value to get value of imm[31:12]
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


int main(int argc, char *argv[]) {
 
	string memoryimage= "program.mem";
	uint32_t max_pc;

    if (argc > 4) {
        cerr << "***CHECK ARGUMENTS*** Incorrect Number of Arguments Provided. Should be Used as follows: \n" << argv[0] << " <pc> <sp> <memoryimage>" << endl;
        return EXIT_FAILURE;
    } else {
		if(argc>1){								// Parse PC and SP as 32-bit hexadecimal values, and Memory Image File as a String
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
    cout << "PC = 0x" << setfill('0') << setw(8) << hex << pc << endl;		// printing the parsed arguments
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
			if(address >= MEM_SIZE){
				cerr << "***ADDRESS OUT OF BOUNDS*** Instruction Memory is limited to 64KB Wrong Address detected in the Memory Image. Stopped at address 0x" << hex << address << endl;
				return 0;
			}
			if(address%4 != 0){
				cerr << "***MEMFILE FORMAT*** Wrong Address detected in the Memory Image. Instruction Address Can only be a multiple of 4. Stopped at address 0x" << hex << address << endl;
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
	//SUBPreload();
	//XORPreload();
	//SRAIPreload();
	//ORIPreload();
	//ADDIPreload();
	LHUPreload();
	//LBPreload();
	//BLTPreload();
	//JALPreload();
	if(pc > (Memory.rbegin() -> first)){
		cerr <<"***WRONG INPUTS*** PC value is not within mem file range.Please enter a valid PC value in the First argument you supply" << endl;
		return 0;
	} else {
		for (pc ; pc < max_pc; pc = pc + 4) {
		uint32_t key = pc;					
		uint32_t value = Memory[key] | (Memory[key + 1] << 8) | (Memory[key + 2] << 16) | (Memory[key + 3] << 24);
		if(value == 0){
			cerr << "***TRAP*** Instruction at address:0x" << hex << key << " is all 0's. Terminating the Simulation" << endl;
			return 0;
		} else {
			uint32_t opcode = value & 0x7F;
				switch(opcode){
					case 0b0110111: if(!UtypeDecode(&Fields,value)) return 1;	break;	// For LUI
					case 0b0010111: if(!UtypeDecode(&Fields,value)) return 1;	break;	// For AUIPC
					case 0b1101111: if(!JtypeDecode(&Fields,value)) return 1;	break;	// For JAL
					case 0b1100111: if(!ItypeDecode(&Fields,value)) return 1;	break;	// For JALR
					case 0b1100011: if(!BtypeDecode(&Fields,value)) return 1;	break;	// For BEQ,BNE,BLT,BGE,BLTU,BGEU
					case 0b0000011: if(!ItypeDecode(&Fields,value)) return 1;	break;	// For LB,LH,LW,LBU,LHU
					case 0b0100011: if(!StypeDecode(&Fields,value)) return 1;	break;	// For SB,SH,SW
					case 0b0010011: if(!ItypeDecode(&Fields,value)) return 1;	break;	// For ADDI,SLTI,SLTIU,ANDI,ORI,XORI,SLLI,SRLI,SRAI
					case 0b0110011: if(!RtypeDecode(&Fields,value)) return 1;	break;	// For ADD,SLT,SLTU,AND,OR,XOR,SLL,SRL,SUB,SRA
					case 0b0001111: break; 							 	// For FENCE
					case 0b1110011: ItypeDecode(&Fields,value);	break;	// For ECALL,EBREAK
					default : 		cerr << "***ILLEGAL OPCODE*** Detected at address(hex): "<< hex << key << "\tInstruction:"<< setfill('0') << setw(8) << hex << value <<"\topcode(binary): " << bitset<7>(opcode) <<"\n"; 
									return 0; break;
				}
				#ifdef VERBOSE 
					print_regs();
				#endif
			}
		}
	}
	#ifdef SILENT 
		print_regs();
	#endif
	#ifdef DEBUGMEM
    print_memory();
	#endif
	
    return EXIT_SUCCESS;

}

// commands to use: g++ -o chk2 chkpt2.cpp ; ./chk2 98 76 file1.mem
