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
	print_regs();
		printf("Fields->imm_U31_12=0x%05x,Fields->rd=0x%02x\n",Fields->imm_U31_12,Fields->rd);
		RegisterFile[Fields->rd] = Fields->imm_U31_12 <<12 & 0xfffff000 ;        
		printf("Fields->imm_U31_12=0x%05x,Fields->rd=0x%02x\n",Fields->imm_U31_12,Fields->rd);
		print_regs();
		return 1;
	};
	uint32_t AUIPC(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		uint32_t extendimm;
		uint32_t effaddress;
		print_regs();
		printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		extendimm = (Fields->imm_U31_12)<<12;
		extendimm = extendimm & 0xFFFFF000;
		RegisterFile[Fields->rd] = pc + extendimm;      //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,extendimm=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rd],extendimm,pc);
		print_regs();
		return 1;
		return 1;
	};
	uint32_t JAL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t JALR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BEQ(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x",Fields->imm_B12_1,Fields->rs2,Fields->rs1);
		int Immediate_Signed;
		if(Fields->imm_B12 >>11) Immediate_Signed = static_cast<int>(Fields->imm_B12 | 0xFFFFF000);
		else Immediate_Signed = static_cast<int>(Fields->imm_B12);
		if(RegisterFile[Fields->rs1]==RegisterFile[Fields->rs2]) pc+=Immediate_Signed - 4;        
		printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x",Fields->imm_B12_1,Fields->rs2,Fields->rs1);
		print_regs();
		return 1;
	};
	uint32_t BNE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		int32_t signextendoffset;
		print_regs();
		printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_B12_1,Fields->rs1,Fields->rs2);
		signextendoffset = int32_t(Fields->imm_B12_1);
		if(RegisterFile[Fields->rs1] == RegisterFile[Fields->rs2])
				pc = pc + 4;
		else	pc = pc + signextendoffset;
		printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rs1],RegisterFile[Fields->rs2],pc);
		print_regs();
		return 1;
	};
	uint32_t BLT(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BGE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t BLTU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x",Fields->imm_B12_1,Fields->rs2,Fields->rs1);
		if(RegisterFile[Fields->rs1]<RegisterFile[Fields->rs2]) pc+=Fields->imm_B12_1 - 4;        
		printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x",Fields->imm_B12_1,Fields->rs2,Fields->rs1);
		print_regs();
		return 1;
	};
	uint32_t BGEU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		int32_t signextendoffset;
		print_regs();
		printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_B12_1,Fields->rs1,Fields->rs2);
		signextendoffset = int32_t(Fields->imm_B12_1);
		if(RegisterFile[Fields->rs1] >= RegisterFile[Fields->rs2])
				pc = pc + signextendoffset;
		else	pc = pc + 4;
		printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rs1],RegisterFile[Fields->rs2],pc);
		print_regs();
		return 1;
	};
	uint32_t LB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
		return 1;
	};
	uint32_t LH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
		return 1;
	};
	uint32_t LW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
		print_regs();
		printf("Fields->imm_I11_0=0x%03x,Fields->rs1=0x%02x,Fields->rd=0x%02x",Fields->imm_I11_0,Fields->rs1,Fields->rd);
		int Immediate_Signed;
		if(Fields->imm_I11_0 >>11) Immediate_Signed = static_cast<int>(Fields->imm_I11_0 | 0xFFFFF000);
		else Immediate_Signed = static_cast<int>(Fields->imm_I11_0);
		RegisterFile[Fields->rd] = Memory[Immediate_Signed+RegisterFile[Fields->rs1]];
		printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x",Fields->imm_B12_1,Fields->rs2,Fields->rs1);
		print_regs();
		return 1;
	};
	uint32_t LBU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
		int32_t signextendimm;
		uint32_t effaddress;
		print_regs();
		printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		signextendimm = int32_t(Fields->imm_S11_0);
		effaddress = RegisterFile[Fields->rs1] + signextendimm;
		RegisterFile[Fields->rd] = Memory[effaddress];       //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x\n",RegisterFile[Fields->rd],effaddress,Memory[effaddress]);
		print_regs();
		return 1;
	};
	uint32_t LHU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
		return 1; 
	};
	uint32_t SB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
		return 1;
	};
	uint32_t SH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
	print_regs();
		printf("Fields->imm_S11_0=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x",Fields->imm_S11_0,Fields->rs2,Fields->rs1);
		int Immediate_Signed;
		if(Fields->imm_S11_0 >>11) Immediate_Signed = static_cast<int>(Fields->imm_S11_0 | 0xFFFFF000);
		else Immediate_Signed = static_cast<int>(Fields->imm_S11_0);
		Memory[RegisterFile[Fields->rs1] + Immediate_Signed] = RegisterFile[Fields->rs2] & 0x0000FFFF;        
		printf("Fields->imm_S11_0=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x",Fields->imm_S11_0,Fields->rs2,Fields->rs1);
		print_regs();
		return 1;
	};
	uint32_t SW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint32_t>& Memory){
		int32_t signextendimm;
		uint32_t effaddress;
		print_regs();
		printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		signextendimm = int32_t(Fields->imm_S11_0);
		effaddress = RegisterFile[Fields->rs1] + signextendimm;
		Memory[effaddress] = RegisterFile[Fields->rs2];       //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rs2]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x\n",RegisterFile[Fields->rs2],effaddress,Memory[effaddress]);
		print_regs();
		return 1;
	};
	uint32_t ADDI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLTI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLTIU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->imm_I11_0=0x%03x,Fields->rs1=0x%02x,Fields->rd=0x%02x",Fields->imm_I11_0,Fields->rs1,Fields->rd);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] < Fields->imm_I11_0 ? 1 : 0;     
		printf("Fields->imm_I11_0=0x%03x,Fields->rs1=0x%02x,Fields->rd=0x%02x",Fields->imm_I11_0,Fields->rs1,Fields->rd);
		print_regs();
		return 1;
	};
	uint32_t XORI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		int32_t signextendimm;
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm_I11_0=0x%03x\n",Fields->rd,Fields->rs1,Fields->imm_I11_0);
		signextendimm = int32_t(Fields->imm_I11_0);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] ^ signextendimm;        //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,signexteneded imm_I11_0=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],signextendimm);
		print_regs();
		return 1;
	};
	uint32_t ORI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t ANDI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		return 1;
	};
	uint32_t SLLI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rs1=0x%02x,Fields->shamt=0x%02x,Fields->rd=0x%02x",Fields->rs1,Fields->shamt,Fields->rd);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] << Fields->shamt;   
		printf("Fields->rs1=0x%02x,Fields->shamt=0x%02x,Fields->rd=0x%02x",Fields->rs1,Fields->shamt,Fields->rd);
		print_regs();
		return 1;
	};
	uint32_t SRLI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->shamt=0x%02x\n",Fields->rd,Fields->rs1,Fields->shamt);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] << RegisterFile[Fields->shamt];        //set_reg(Fields->rd,RegisterFile[Fields->rd]);
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->shamt]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->shamt]);
		print_regs();
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
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] < RegisterFile[Fields->rs2] ? 1 : 0;        
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		print_regs();
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
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		int32_t signed_rs1 = static_cast<int32_t>(RegisterFile[Fields->rs1]);
		RegisterFile[Fields->rd] = signed_rs1 << RegisterFile[Fields->rs2];        
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		print_regs();
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