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
	
	uint32_t LUI(){
		return 1;
	};
	uint32_t AUIPC(){
		return 1;
	};
	uint32_t JAL(){
		return 1;
	};
	uint32_t JALR(){
		return 1;
	};
	uint32_t BEQ(){
		return 1;
	};
	uint32_t BNE(){
		return 1;
	};
	uint32_t BLT(){
		return 1;
	};
	uint32_t BGE(){
		return 1;
	};
	uint32_t BLTU(){
		return 1;
	};
	uint32_t BGEU(){
		return 1;
	};
	uint32_t LB(){
		return 1;
	};
	uint32_t LH(){
		return 1;
	};
	uint32_t LW(){
		return 1;
	};
	uint32_t LBU(){
		return 1;
	};
	uint32_t LHU(){
		return 1; 
	};
	uint32_t SB(){
		return 1;
	};
	uint32_t SH(){
		return 1;
	};
	uint32_t SW(){
		return 1;
	};
	uint32_t ADDI(){
		return 1;
	};
	uint32_t SLTI(){
		return 1;
	};
	uint32_t SLTIU(){
		return 1;
	};
	uint32_t XORI(){
		return 1;
	};
	uint32_t ORI(){
		return 1;
	};
	uint32_t ANDI(){
		return 1;
	};
	uint32_t SLLI(){
		return 1;
	};
	uint32_t SRLI(){
		return 1;
	};
	uint32_t SRAI(){
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
	
	uint32_t SUB(){
		return 1;
	};
	uint32_t SLL(){
		return 1;
	};
	uint32_t SLT(){
		return 1;
	};
	uint32_t SLTU(){
		return 1;
	};
	uint32_t XOR(){
		return 1;
	};
	uint32_t SRL(){
		return 1;
	};
	uint32_t SRA(){
		return 1;
	};
	uint32_t OR(){
		return 1;
	};
	uint32_t AND(){
		return 1;
	};
	uint32_t FENCE(){
		return 1;
	};
	uint32_t ECALL(){
		return 1;
	};
	uint32_t EBREAK(){
		return 1;
	};
