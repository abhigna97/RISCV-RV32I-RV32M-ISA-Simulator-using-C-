using namespace std;
uint32_t pc=0,sp=0,ra=0;												// Modeling Program Counter(0), Stack Pointer(0), Return Address(0)
map <uint16_t, uint8_t> Memory;       									// Modelling the Data Memory

const char *reg_names[] = {												// Modeling the 32 32-bit GPRs
            "x0/zero", "x1/ra", "x2/sp", "x3/gp", "x4/tp", "x5/t0", "x6/t1", "x7/t2",
            "x8/s0", "x9/s1", "x10/a0", "x11/a1", "x12/a2", "x13/a3", "x14/a4", "x15/a5",
            "x16/a6", "x17/a7", "x18/s2", "x19/s3", "x20/s4", "x21/s5", "x22/s6", "x23/s7",
            "x24/s8", "x25/s9", "x26/s10", "x27/s11", "x28/t3", "x29/t4", "x30/t5", "x31/t6"};
			
const uint32_t MEM_SIZE  = 65536;				  						// Size of Memory Restricted to 64 KB 
const uint32_t WORD_SIZE = 8;    										// SIze of Word = 8 bytes
const uint32_t NUM_REGS  = sizeof(reg_names) / sizeof(reg_names[0]);	// Calculate the number of registers: 32 in RISC-V

map<uint32_t, uint32_t> RegisterFile{									// Modeling the 32 RISCV General Purpose Registers
	{0, 0}, {1, ra}, {2, sp}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
    {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0},
    {16, 0}, {17, 0}, {18, 0}, {19, 0}, {20, 0}, {21, 0}, {22, 0}, {23, 0},
    {24, 0}, {25, 0}, {26, 0}, {27, 0}, {28, 0}, {29, 0}, {30, 0}, {31, 0}
};
struct InstrFields{					//Struct type for Instruction that holds different part of the Instruction
    unsigned int opcode;         	//opcode
    unsigned int rs1;            	//rs1- source register1
    unsigned int rs2;           	//rs2- source register2
    unsigned int rd;             	//rd - destination register
    unsigned int funct3;        	//Funct 3 
    unsigned int funct7;        	//funct 7
    unsigned int imm_I11_0;         //Immediate value for I Instruction format
	unsigned int imm_I11_5;         //Imm[11:5] value for I Instruction format
	unsigned int shamt;				//shamt(Imm[4:0]) in place of rs2 for R-type SLLI,SRLI,SRAI
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
	unsigned int fm;				//fm for FENCE
	unsigned int pred;				//pred for FENCE
	unsigned int succ;				//succ for FENCE
};

InstrFields Fields = {0};