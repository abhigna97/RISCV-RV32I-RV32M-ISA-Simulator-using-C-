void set_pc(uint32_t pcvalue) { pc = pcvalue; }												// Getters and setters for the program counter
uint32_t get_pc() { return pc; }

void set_reg(uint32_t regindex, uint32_t regvalue) { RegisterFile[regindex] = regvalue; }	// Getters and setters for general-purpose registers
uint32_t get_reg(uint32_t regindex) { return RegisterFile[regindex]; }

void mem_write(uint32_t address, uint32_t data) {											// Memory read operation
	for (uint32_t i = 0; i < WORD_SIZE; i++) {
		Memory[address + i] = (data >> (i * 8)) & 0xffffffff;
	}
}

uint32_t mem_read(uint32_t address) {														// Memory write operation
	uint32_t data = 0;
	for (uint32_t i = 0; i < WORD_SIZE; i++) {
		data |= ((uint32_t)Memory[address + i]) << (i * 8);
	}
	return data;
}

void initialize() {																			// initialize PC and all GPRs with a given value
	pc = 0;
	for (uint32_t i = 0; i < NUM_REGS; i++) {
		RegisterFile[i] = 0;
	}
}

void print_regs() {																			// Function to Print the register information in a compact format with register names and values
	printf("PC\t:0x%08x\n", pc);
	for (uint32_t i = 0; i < NUM_REGS; i++) {
		printf("%s\t:0x%08x  ", reg_names[i], RegisterFile[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}
	printf("\n");
}

void print_memory() {																		// Function to Print the memory information in a compact format with register names and values
	for (uint32_t i = 0; i < MEM_SIZE; i = i + 4) {
		printf("%x:\t0x%08x\t", i, Memory[i]);
		if ((i + 4) % 24 == 0) {
			printf("\n");
		}
	}
	printf("\n");
}

uint32_t LUI() {
	return 1;
};
uint32_t AUIPC() {
	return 1;
};
uint32_t JAL(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
	int32_t signextendoffset;
	uint32_t effectiveaddress;
	print_regs();
	printf("Fields->imm_J20_1=0x%03x,PC=0x%08x,Fields->rd=0x%02x\n", Fields->imm_J20_1, pc, Fields->rd);
	signextendoffset = int32_t(Fields->imm_J20_1);
	effectiveaddress = pc + signextendoffset;
	RegisterFile[Fields->rd] = pc + 4;
	printf("PC=0x%08x,RegisterFile[Fields->rd]=0x%08x\n", pc, RegisterFile[Fields->rd]);
	print_regs();
	return 1;
};
uint32_t JALR() {
	return 1;
};
uint32_t BEQ() {
	return 1;
};
uint32_t BNE() {
	return 1;
};
uint32_t BLT(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
	int32_t signextendoffset;
	print_regs();
	printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x,PC=0x%08x\n", Fields->imm_B12_1, Fields->rs1, Fields->rs2, pc);
	signextendoffset = int32_t(Fields->imm_B12_1);
	if (RegisterFile[Fields->rs1] < RegisterFile[Fields->rs2])
		pc = pc + signextendoffset;
	else    pc = pc + 4;
	printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x\n", RegisterFile[Fields->rs1], RegisterFile[Fields->rs2], pc);
	print_regs();
	return 1;
};
uint32_t BGE() {
	return 1;
};
uint32_t BLTU() {
	return 1;
};
uint32_t BGEU() {
	return 1;
};
uint32_t LB(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile, map<uint32_t, uint32_t>& Memory) {
	print_regs();
	printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
	uint32_t signedbit;
	if (Fields->imm_I11_0 >> 11) signedbit = (Fields->imm_I11_0 | 0xFFFFF000);
	else uint32_t signedbit = (Fields->imm_I11_0 | 0x000FFFFF);
	uint32_t resultW = Memory [RegisterFile[Fields->rs1] + signedbit];
	uint32_t resultB = resultW & 0x000000FF;
	uint32_t result32;
	if (resultB & 0x80) {
		result32 = resultB | 0xFFFFFF00;
	}
	else {
		result32 = resultB | 0x000000FF;
	}
	RegisterFile[Fields->rd] = result32;
	printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,resultB, result32 \n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], signedbit, resultW, resultB, result32);
	print_regs();
return 1;
	};
	uint32_t LH(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile, map<uint32_t, uint32_t>& Memory) {
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		uint32_t signedbit;
		if (Fields->imm_I11_0 >> 11) signedbit = (Fields->imm_I11_0 | 0xFFFFF000);
		else uint32_t signedbit = (Fields->imm_I11_0 | 0x000FFFFF);
		uint32_t resultW = Memory [RegisterFile[Fields->rs1] + signedbit];
		uint32_t resultHW = resultW & 0x0000FFFF;
		uint32_t result32;
		if (resultHW & 0x8000) {
			result32 = resultHW | 0xFFFF0000;
		}
		else {
			result32 = resultHW | 0x0000FFFF;
		}
		RegisterFile[Fields->rd] = result32;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,resultHW, result32 \n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], signedbit, resultW, resultHW, result32);
		print_regs();
		return 1;
	};
	uint32_t LW() {
		return 1;
	};
	uint32_t LBU() {
		return 1;
	};
	uint32_t LHU() {
		return 1;
	};
	uint32_t SB() {
		return 1;
	};
	uint32_t SH() {
		return 1;
	};
	uint32_t SW() {
		return 1;
	};
	uint32_t ADDI(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] + Fields->imm_I11_0;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1]);
		print_regs();
	return 1;
	};
	uint32_t SLTI() {
		return 1;
	};
	uint32_t SLTIU() {
		return 1;
	};
	uint32_t XORI() {
		return 1;
	};
	uint32_t ORI(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm_I11_0=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] | Fields->imm_I11_0;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1]);
		print_regs();
		return 1;
	};
	uint32_t ANDI() {
		return 1;
	};
	uint32_t SLLI() {
		return 1;
	};
	uint32_t SRLI() {
		return 1;
	};
	uint32_t SRAI(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->shamt=0x%02x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		uint32_t signedbit = RegisterFile[Fields->rs1] & 0x80000000;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] >> Fields->shamt;
		RegisterFile[Fields->rd] |= signedbit;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x, signedbit=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], signedbit);
		print_regs();
		return 1;
	};

	uint32_t ADD(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] + RegisterFile[Fields->rs2];        //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
		print_regs();
		return 1;
	};

	uint32_t SUB(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] - RegisterFile[Fields->rs2];        //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
		print_regs();
		return 1;
	};
	uint32_t SLL() {
		return 1;
	};
	uint32_t SLT() {
		return 1;
	};
	uint32_t SLTU() {
		return 1;
	};
	uint32_t XOR(struct InstrFields* Fields, map<uint32_t, uint32_t>& RegisterFile) {
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] ^ RegisterFile[Fields->rs2];
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
		print_regs();
	return 1;
	};
	uint32_t SRL() {
		return 1;
	};
	uint32_t SRA() {
		return 1;
	};
	uint32_t OR() {
		return 1;
	};
	uint32_t AND() {
		return 1;
	};
	uint32_t FENCE() {
		return 1;
	};
	uint32_t ECALL() {
		return 1;
	};
	uint32_t EBREAK() {
		return 1;
	};
