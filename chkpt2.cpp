#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h>

using namespace std;
uint32_t pc=0,sp=0,ra=0;												// Modeling Program Counter, Stack Pointer, Return Address
map <uint32_t, uint32_t> Memory;       									// Modelling the Memory
/* map <string,uint32_t> RegisterFile {									// Modeling the 32 RISCV General Purpose Registers
	{"zero", 0}, {"ra", ra}, {"sp", sp}, {"gp", 0}, {"tp", 0}, {"t0", 0}, {"t1", 0}, {"t2", 0},
    {"s0", 0}, {"s1", 0}, {"a0", 0}, {"a1", 0}, {"a2", 0}, {"a3", 0}, {"a4", 0}, {"a5", 0},
    {"a6", 0}, {"a7", 0}, {"s2", 0}, {"s3", 0}, {"s4", 0}, {"s5", 0}, {"s6", 0}, {"s7", 0},
    {"s8", 0}, {"s9", 0}, {"s10", 0}, {"s11", 0}, {"t3", 0}, {"t4", 0}, {"t5", 0}, {"t6", 0}
}; */

using KeyType = variant<string, uint32_t>;
unordered_map<KeyType, uint32_t> RegisterFile{									// Modeling the 32 RISCV General Purpose Registers
	{"zero", 0}, {"ra", ra}, {"sp", sp}, {"gp", 0}, {"tp", 0}, {"t0", 0}, {"t1", 0}, {"t2", 0},
    {"s0", 0}, {"s1", 0}, {"a0", 0}, {"a1", 0}, {"a2", 0}, {"a3", 0}, {"a4", 0}, {"a5", 0},
    {"a6", 0}, {"a7", 0}, {"s2", 0}, {"s3", 0}, {"s4", 0}, {"s5", 0}, {"s6", 0}, {"s7", 0},
    {"s8", 0}, {"s9", 0}, {"s10", 0}, {"s11", 0}, {"t3", 0}, {"t4", 0}, {"t5", 0}, {"t6", 0}
};

#include "functions.h"

const char *reg_names[] = {												// Modeling the 32 32-bit GPRs
            "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
            "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
            "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
            "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

const uint32_t MEM_SIZE  = 65536;  										// Size of Memory = 64 KB
const uint32_t WORD_SIZE = 8;    										// SIze of Word = 8 bytes
const uint32_t NUM_REGS  = sizeof(reg_names) / sizeof(reg_names[0]);	// Calculate the number of registers: 32 in RISC-V

struct InstrFields{				//Struct type for Instruction that holds different part of the Instruction
    unsigned int opcode;         	//opcode
    unsigned int rs1;            	//rs1- source register1
    unsigned int rs2;           	//rs2- source register2
    unsigned int rd;             	//rd - destination register
    unsigned int funct3;        	//Funct 3 
    unsigned int funct7;        	//funct 7
    unsigned int imm_I11_0;         //Immediate value for I Instruction format
    unsigned int imm_S4_0;         	//Imm[4:0] for S type format
    unsigned int imm_S11_5;         //Imm[11:5] for S type format
    unsigned int imm_S11_0;         //Imm[11:0] for S type format
    unsigned int imm_B11;			//Imm[11] for B type format
	unsigned int imm_B4_1;			//Imm[4:1] for B type format
	unsigned int imm_B10_5;			//Imm[10:5] for B type format
	unsigned int imm_B12;			//Imm[12] for B type format
	unsigned int imm_B12_1;			//Imm[12:1] for B type format
	unsigned int imm_U31_12;		//Imm[31:12] for U type format
    unsigned int imm_J19_12;		//Imm[19:12] for J type format
	unsigned int imm_J11;			//Imm[11] for J type format
	unsigned int imm_J10_1;			//Imm[10:1] for J type format
	unsigned int imm_J20;			//Imm[20] for J type format
	unsigned int imm_J20_1;			//Imm[20:1] for J type format
	unsigned int shamt;				//shamt in place of rs2 for R-type SLLI,SRLI,SRAI
	unsigned int fm;				//fm for FENCE
	unsigned int pred;				//pred for FENCE
	unsigned int succ;				//succ for FENCE
};

InstrFields Fields = {0};

void RtypeExecute(struct InstrFields *Fields, uint32_t instruction, uint32_t temp_arr[]){
	uint32_t funct3temp,funct7temp;
	funct3temp = Fields->funct3;
	funct7temp = Fields->funct7;
    switch(funct7temp){
				case 0b0000000: switch (funct3temp){
									case 0b000: ADD(Fields);break;
									case 0b001: SLL();	break;
									case 0b010: SLT();	break;
									case 0b011: SLTU();	break;
									case 0b100: XOR();	break;
									case 0b101: SRL();	break;
									case 0b110: OR();	break;
									case 0b111: AND();	break;
								} break;
				case 0b0100000: switch (funct3temp){
									case 0b000: SUB();break;
									case 0b101: SRA();break;
								} break;
				default : break;
	}
}

void ItypeExecute(struct InstrFields *Fields, uint32_t instruction, uint32_t temp_arr[]){
	uint32_t funct3temp;
	funct3temp = Fields->funct3;
    switch(funct3temp){
				case 0b000: LB(); break;
				case 0b001: LH(); break;
				case 0b010: LW(); break;
				case 0b100: LBU(); break;
				case 0b101: LHU(); break;
				default : break;
	}
}

void StypeExecute(struct InstrFields *Fields, uint32_t instruction, uint32_t temp_arr[]){
	uint32_t funct3temp;
	switch(funct3temp){
				case 0b000: SB(); break;		
				case 0b001: SH(); break;
				case 0b010: SW(); break;
				default : break;
	}
}

void BtypeExecute(struct InstrFields *Fields, uint32_t instruction, uint32_t temp_arr[]){
	uint32_t funct3temp;
	funct3temp = Fields->funct3;
	switch(funct3temp){
				case 0b000: BEQ(); break;
				case 0b001: BNE(); break;
				case 0b100: BLT(); break;
				case 0b101: BGE(); break;
				case 0b110: BLTU(); break;
				case 0b111: BGEU(); break;
				default : break;
	}
}

void UtypeExecute(struct InstrFields *Fields, uint32_t instruction, uint32_t temp_arr[]){
	uint32_t opcodetemp;
	opcodetemp = Fields->opcode;
    switch(opcodetemp){
				case 0b0110111: LUI(); break;
				case 0b0010111: AUIPC(); break;
				default : break;
	}
}

void JtypeExecute(struct InstrFields *Fields, uint32_t instruction, uint32_t temp_arr[]){
	JAL();
}

void RtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,array[]={};					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000003F; 		// masking instrtemp with mask value to get value of Opcode
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
	printf("****R-TYPE:0x%08x****\nfunct7\t=0x%02x\n,rs2\t=0x%02x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->funct7,Fields->rs2,Fields->rs1,Fields->funct3,Fields->rd,Fields->opcode);
	RtypeExecute(Fields, instruction, array);
}

void ItypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,array[]={}; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000003F; 		// masking instrtemp with mask value to get value of Opcode
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
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get imm[11:0]
    Fields->imm_I11_0 	= instrtemp & 0x00000FFF; 	// masking instrtemp with a mask value to get value of imm[11:0]
    printf("****I-TYPE:0x%08x****\nimm[11:0]\t=0x%03x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_I11_0,Fields->rs1,Fields->funct3,Fields->rd,Fields->opcode);
	ItypeExecute(Fields, instruction, array);
}

void StypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,array[]={}; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000003F; 		// masking instrtemp with mask value to get value of Opcode
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
	printf("****S-TYPE:0x%08x****\nimm[11:5]\t=0x%02x\n,rs2\t=0x%02x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,imm[4:0]\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_S11_5,Fields->rs2,Fields->rs1,Fields->funct3,Fields->imm_S4_0,Fields->opcode);
	StypeExecute(Fields, instruction, array);
}

void BtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,array[]={}; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000003F; 		// masking instrtemp with mask value to get value of Opcode
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
	printf("****B-TYPE:0x%08x****\nimm[12]\t=%0x\n,imm[10:5]\t=%0x\n,rs2\t=%0x\n,rs1\t=%0x\n,funct3\t=%0x\n,imm[4:1]\t=%0x\n,imm[11]\t=%0x\n,opcode\t=%0x\n",instruction,Fields->imm_B12,Fields->imm_B10_5,Fields->rs2,Fields->rs1,Fields->funct3,Fields->imm_B4_1,Fields->imm_B11,Fields->opcode);
	BtypeExecute(Fields, instruction, array);
}

void UtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,array[]={}; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000003F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get imm[31:12]
    Fields->imm_U31_12 	= instrtemp & 0x000FFFFF; 		// masking instrtemp with a mask value to get value of imm[31:12]
	printf("****U-TYPE:0x%08x****\nimm[31:12]\t=0x%05x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_U31_12,Fields->rd,Fields->opcode);
	UtypeExecute(Fields, instruction, array);
}

void JtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,array[]={}; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000003F; 		// masking instrtemp with mask value to get value of Opcode
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
	printf("****J-TYPE:0x%08x****\nimm[20]\t=0x%01x\n,imm[10:1]\t=0x%03x\n,imm[11]\t=0x%01x\n,imm[19:12]\t=0x%02x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_J20,Fields->imm_J10_1,Fields->imm_J11,Fields->imm_J19_12,Fields->rd,Fields->opcode);
	JtypeExecute(Fields, instruction, array);
}

/* class RISCV {
	
private:
    //uint32_t pc = 0;													// Modeling Program counter
   

public: */
    
    void set_pc(uint32_t pcvalue) { pc = pcvalue; }											// Getters and setters for the program counter
    uint32_t get_pc() /* const */ { return pc; }

    void set_reg(string regname, uint32_t regvalue) { RegisterFile[regname] = regvalue; }	// Getters and setters for general-purpose registers
    uint32_t get_reg(string regname) /* const */ { return RegisterFile[regname]; }

    void mem_write(uint32_t address, uint32_t data) {										// Memory read operation
        for (uint32_t i = 0; i < WORD_SIZE; i++) {
            Memory[address + i] = (data >> (i * 8)) & 0xffffffff;
        }
    }
    uint32_t mem_read(uint32_t address) /* const */ {												// Memory write operation
        uint32_t data = 0;
        for (uint32_t i = 0; i < WORD_SIZE; i++) {
            data |= ((uint32_t) Memory[address + i]) << (i * 8);
        }
        return data;
    }
	
	void initialize(){																		// initialize PC and all GPRs with a given value
		pc = 0;
		for(uint32_t i = 0; i < NUM_REGS; i++){										
	    RegisterFile[reg_names[i]] = 0;}
	}

    void print_regs() /* const */ {																// Function to Print the register information in a compact format with register names and values
        printf("PC:\t0x%08x\n",pc);
        for (uint32_t i = 0; i < NUM_REGS; i++) {
            printf("%s:\t0x%08x ", reg_names[i], RegisterFile[reg_names[i]]);
            if ((i + 1) % 4 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }
	
	void print_memory() /* const */ {																// Function to Print the memory information in a compact format with register names and values
        for (uint32_t i = 0; i < MEM_SIZE; i=i+4) {
            printf("%x:\t0x%08x\t", i, Memory[i]);
            if ((i+4) % 24 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }
	
/* }; */

/*uint32_t InstructionDecode(uint32_t Instruction, uint32_t PCValue){
};*/


int main(int argc, char *argv[]) {
    /* RISCV cpu; */   			 			// Create a RISC-V CPU instance
    string memoryimage= "program.mem";

    if (argc > 4) {
        cerr << "*****Incorrect Number of Arguments Provided. Should be Used as follows: \n" << argv[0] << " <pc> <sp> <memoryimage>" << endl;
        return EXIT_FAILURE;
    } else {
		if(argc>1){								// Parse PC and SP as 32-bit hexadecimal values, and Memory Image File as a String
			stringstream ss_pc(argv[1]);
			ss_pc >> hex >> pc;
		}
		if(argc>2){
			stringstream ss_sp(argv[2]);
			ss_sp >> hex >> sp;
		} 	
		if(argc>3){
			memoryimage = string(argv[3]);
		}	
	}

    cout << "PC = 0x" << setfill('0') << setw(8) << hex << pc << endl;		// printing the parsed arguments
    cout << "SP = 0x" << setfill('0') << setw(8) << hex << sp << endl;
    cout << "Memory image filename = " << memoryimage << endl;
	
	ifstream memory_file(memoryimage);										// Loading the Memory Image into Memory Map
	uint32_t address,data;
	if (memory_file.is_open()) {
        string line;
        while (getline(memory_file, line)) {
            stringstream ss(line);
            ss >> hex >> address;
            ss.ignore(2, ' '); 												// Ignore the colon and space between address and data
            ss >> hex >> data;
            Memory[address] = data;
			uint32_t opcode = data & 0x3F;
			switch(opcode){
				case 0b0110111: UtypeDecode(&Fields,data);break;// For LUI
				case 0b0010111: UtypeDecode(&Fields,data);break;// For AUIPC
				case 0b1101111: JtypeDecode(&Fields,data);break;// For JAL
				case 0b1100111: ItypeDecode(&Fields,data);break;// For JALR
				case 0b1100011: BtypeDecode(&Fields,data);break;// For BEQ,BNE,BLT,BGE,BLTU,BGEU
				case 0b0000011: ItypeDecode(&Fields,data);break;// For LB,LH,LW,LBU,LHU
				case 0b0100011: StypeDecode(&Fields,data);break;// For SB,SH,SW
				case 0b0010011: ItypeDecode(&Fields,data);break;// For ADDI,SLTI,SLTIU,ANDI,ORI,XORI,SLLI,SRLI,SRAI
				case 0b0110011: RtypeDecode(&Fields,data);break;// For ADD,SLT,SLTU,AND,OR,XOR,SLL,SRL,SUB,SRA
				case 0b0001111: break; 							// For FENCE
				case 0b1110011: ItypeDecode(&Fields,data);break;// For ECALL,EBREAK
				default : break;
			}
        }
        memory_file.close();
    } else {
        cerr << "Failed to open memory image file." << endl;
        return 0;
    }
	
 	//cpu.set_pc(0x0);														// Set PC
    //RegisterFile["sp"] = sp;												// Set SP
    //RegisterFile["ra"] = ra;   											// Set RA
	/* cpu.set_pc(pc);															// Set PC
	cpu.set_reg("sp",sp);													// Set SP
	cpu.set_reg("ra",ra);													// Set RA
	cpu.set_reg("t0",0x12345678);											// Set a Register's Value
	cpu.get_reg("t0");	 */													// Get a Register's Value
	set_pc(pc);															// Set PC
	set_reg("sp",sp);													// Set SP
	set_reg("ra",ra);													// Set RA
	set_reg("t0",0x12345678);											// Set a Register's Value
	get_reg("t0");	
    //cpu.initialize();														// initialize PC and Registers
    //cpu.mem_write(0xfffe, 0x12345678);									// Write data to a memory location
    //cpu.mem_write(0xffbc, 0xdeadbeef);
    /* cpu.print_regs(); */														// Print the Registers and their Contents
	print_regs();
	//cpu.print_memory();													// Print the Memory Locations and their Contents


    return EXIT_SUCCESS;

}

// commands to use: g++ -o chk2 chkpt2.cpp ; ./chk2 98 76 program.mem
