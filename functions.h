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
            data |= ((uint32_t) Memory[address + i]) << (i * 8);
        }
        return data;
    }
	
	void initialize(){																			// initialize PC and all GPRs with a given value
		pc = 0;
		for(uint32_t i = 0; i < NUM_REGS; i++){										
	    RegisterFile[i] = 0;}
	}

    void print_regs() {																			// Function to Print the register information in a compact format with register names and values
        printf("PC\t:0x%08x\n",pc);
        for (uint32_t i = 0; i < NUM_REGS; i++) {
            printf("%s\t:0x%08x  ", reg_names[i], RegisterFile[i]);
            if ((i + 1) % 4 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }
	
	void print_memory() {																		// Function to Print the memory information in a compact format with register names and values
        for (uint32_t i = 0; i < MEM_SIZE; i=i+4) {
            printf("%x:\t0x%08x\t", i, Memory[i]);
            if ((i+4) % 24 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }
	
	uint32_t LUI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t AUIPC(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t JAL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t JALR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BEQ(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BNE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BLT(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BGE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BLTU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BGEU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t LB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t LH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t LW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t LBU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t LHU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1; 
	};
	uint32_t SB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t ADDI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLTI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLTIU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t XORI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t ORI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t ANDI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLLI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SRLI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SRAI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	
	uint32_t ADD(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] + RegisterFile[Fields->rs2];        //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
		print_regs();
		return 1;
	};
	
	uint32_t SUB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLT(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLTU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		if(RegisterFile[Fields->rs1] < RegisterFile[Fields->rs2])
				RegisterFile[Fields->rd] = 1;        //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		else	RegisterFile[Fields->rd] = 0;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
		print_regs();
		return 1;
	};
	uint32_t XOR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SRL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SRA(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t OR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] | RegisterFile[Fields->rs2];        //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
		print_regs();
		return 1;
	};
	uint32_t AND(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t FENCE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t ECALL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t EBREAK(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
