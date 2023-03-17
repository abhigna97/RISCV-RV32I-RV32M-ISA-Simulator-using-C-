    void set_pc(uint32_t pcvalue) { pc = pcvalue; }												// Getters and setters for the program counter
    uint32_t get_pc() { return pc; }

    void set_reg(uint32_t regindex, uint32_t regvalue) { RegisterFile[regindex] = regvalue; }	// Getters and setters for general-purpose registers
    uint32_t get_reg(uint32_t regindex) { return RegisterFile[regindex]; }

    void mem_write(uint32_t address, uint8_t data) {											// Memory read operation
        Memory[address] = data;
    }
	
    uint8_t mem_read(uint32_t address) {														// Memory write operation
        uint8_t data ;
 		data = Memory[address];
        return data;
    }
	
	void print_memory() {																		// Function to Print the memory information in a compact format with register names and values
		for (uint32_t i = 0; i < MEM_SIZE; i++) {
            printf("%x:\t0x%02x\t", i, Memory[i]);
            if ((i+1) % 8 == 0) {
                printf("\n");
            }
        }
        printf("\n");
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
		RegisterFile[Fields->rd] = pc + extendimm;      
		printf("RegisterFile[Fields->rd]=0x%08x,extendimm=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rd],extendimm,pc);
		print_regs();
		return 1;
	};
	uint32_t JAL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
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
	uint32_t JALR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
	int32_t signextendoffset;
		uint32_t effaddress;
		print_regs();
		printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rd=0x%02x\n",Fields->imm_B12_1,Fields->rs1,Fields->rd);
		signextendoffset = int32_t(Fields->imm_B12_1);
		effaddress = RegisterFile[Fields->rs1] + signextendoffset;
		effaddress = effaddress << 1;                                  // appending LSB as 0
		RegisterFile[Fields->rd] = pc + 4;
		printf("PC=0x%08x,RegisterFile[Fields->rs1]=0x%02x,RegisterFile[Fields->rd]=0x%02x\n",pc,RegisterFile[Fields->rs1],RegisterFile[Fields->rd]);
		print_regs();
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
	uint32_t BGE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
	int32_t signextendoffset;
                print_regs();
                printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x,PC=0x%08x\n",Fields->imm_B12_1,Fields->rs1,Fields->rs2,pc);
                signextendoffset = int32_t(Fields->imm_B12_1);
                if(RegisterFile[Fields->rs1] >= RegisterFile[Fields->rs2])
                                pc = pc + signextendoffset;
                else    pc = pc + 4;
                printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rs1],RegisterFile[Fields->rs2],pc);
                print_regs();
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
	uint32_t LB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
		print_regs();
        printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		int32_t leftshiftimm= (Fields->imm_I11_0) << 20;
		int32_t signed_imm = static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm = signed_imm >> 20;
        uint32_t effaddress = RegisterFile[Fields->rs1] + signextendimm;
		uint32_t mem8bit = Memory[effaddress];
		int32_t leftshiftbyte= (mem8bit) << 24;
		int32_t signed_byte = static_cast<int32_t>(leftshiftbyte);
		int32_t signextendbyte = signed_byte >> 24;
		RegisterFile[Fields->rd] = signextendbyte;   
        printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x,mem8bit=0x%02x\n",RegisterFile[Fields->rs1], RegisterFile[Fields->rd],effaddress,Memory[effaddress],mem8bit);
        print_regs();
		return 1; 
	};
	uint32_t LH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
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
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,signedbit=0x%08x,resultW=0x%08x,resultHW=0x%04x, result32=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], signedbit, resultW, resultHW, result32);
		print_regs();
		return 1;
	};
	uint32_t LW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
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
	uint32_t LBU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
		int32_t signextendimm;
		uint32_t effaddress;
		print_regs();
		printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		signextendimm = int32_t(Fields->imm_S11_0);
		effaddress = RegisterFile[Fields->rs1] + signextendimm;
		RegisterFile[Fields->rd] = Memory[effaddress];
		printf("RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x\n",RegisterFile[Fields->rd],effaddress,Memory[effaddress]);
		print_regs();
		return 1;
	};
	uint32_t LHU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
		print_regs();
        printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		int32_t leftshiftimm= (Fields->imm_I11_0) << 20;
		int32_t signed_imm = static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm = signed_imm >> 20;
        uint32_t effaddress = RegisterFile[Fields->rs1] + signextendimm;
		uint16_t mem16bit = Memory[effaddress] | (Memory[effaddress + 1] << 8);
		RegisterFile[Fields->rd] = static_cast<uint32_t>(mem16bit);
        printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x,mem16bit=0x%08x\n",RegisterFile[Fields->rs1], RegisterFile[Fields->rd],effaddress,Memory[effaddress],mem16bit);
        print_regs();
		return 1; 
	};
	uint32_t SB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
	print_regs();
		int32_t signextendimm;
		uint32_t effaddress;
		printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		signextendimm = int32_t(Fields->imm_S11_0);
		effaddress = RegisterFile[Fields->rs1] + signextendimm;
		Memory[effaddress] = RegisterFile[Fields->rs2];	
		printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x\n",RegisterFile[Fields->rs1], RegisterFile[Fields->rs2],effaddress,Memory[effaddress]);
		print_regs();
		return 1;
	};
	uint32_t SH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
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
	uint32_t SW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint32_t, uint8_t>& Memory){
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
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		int32_t leftshiftimm= (Fields->imm_I11_0) << 20;
		int32_t signed_imm = static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm = signed_imm >> 20;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] + signextendimm;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,signextendimm=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1],signextendimm);
		print_regs();
		return 1;
	};
	uint32_t SLTI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("RegisterFile[Fields->rs1]=0x%03x,RegisterFile[Fields->rd]=0x%02x\n",RegisterFile[Fields->rs1],RegisterFile[Fields->rd]);
		int32_t leftshiftimm = (Fields->imm_I11_0) << 20;
		int32_t signed_imm = static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm = signed_imm >> 20;
		RegisterFile[Fields->rd] = (int32_t(RegisterFile[Fields->rs1]) < signextendimm) ? 1 : 0; 
		printf("RegisterFile[Fields->rs1]=0x%03x,signextendimm=0x%02x,RegisterFile[Fields->rd]=0x%02x\n",RegisterFile[Fields->rs1],signextendimm,RegisterFile[Fields->rd]);
		print_regs();
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
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm_I11_0=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		int32_t leftshiftimm= (Fields->imm_I11_0) << 20;
		int32_t signed_imm = static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm = signed_imm >> 20;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] | signextendimm;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,signextendimm=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1],signextendimm);
		print_regs();
		return 1;
	};
	uint32_t ANDI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		printf("ANDI Instruction");
	        print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm_I11_0=0x%02x\n",Fields->rd,Fields->rs1,Fields->imm_I11_0);
		int32_t leftshiftimm= (Fields->imm_I11_0) << 20;
		int32_t signed_imm = static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm = signed_imm >> 20;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] & signextendimm;        
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,Fields->imm_I11_0=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],Fields->imm_I11_0);
		print_regs();
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
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] << RegisterFile[Fields->shamt];        
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->shamt]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->shamt]);
		print_regs();
		return 1;
	};
	uint32_t SRAI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->shamt=0x%02x\n", Fields->rd, Fields->rs1, Fields->shamt);
		int32_t signed_rs1 = static_cast<int32_t>(RegisterFile[Fields->rs1]);
		RegisterFile[Fields->rd] =  signed_rs1 >> Fields->shamt;
		printf("RegisterFile[Fields->shamt]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rd]=0x%08x, signed_rs1=0x%08x\n", RegisterFile[Fields->shamt], RegisterFile[Fields->rs1],RegisterFile[Fields->rd], signed_rs1);
		print_regs();
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
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		RegisterFile[Fields->rd] = static_cast<int32_t>(RegisterFile[Fields->rs1]) - static_cast<int32_t>(RegisterFile[Fields->rs2]); 
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd],RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
		print_regs();
		return 1;
	};
	uint32_t SLL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		uint32_t rs2_shamt = RegisterFile[Fields->rs2] & 0x0000001F;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] << rs2_shamt;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n, rs2_shamt = 0x%02x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2], rs2_shamt);
		print_regs();
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
				RegisterFile[Fields->rd] = 1;
		else	RegisterFile[Fields->rd] = 0;
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
		print_regs();
		return 1;
	};
	uint32_t XOR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		print_regs();
		printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] ^ RegisterFile[Fields->rs2];
		printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
		print_regs();
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
	print_regs();
	printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
	RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] & RegisterFile[Fields->rs2];        
	printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
	print_regs();
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
