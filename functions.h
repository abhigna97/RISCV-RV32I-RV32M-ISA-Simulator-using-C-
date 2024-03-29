    void set_pc(uint32_t pcvalue) { pc = pcvalue; }												// Getters and setters for the program counter
    uint32_t get_pc() { return pc; }

    void set_reg(uint32_t regindex, uint32_t regvalue) { RegisterFile[regindex] = regvalue; }	// Getters and setters for general-purpose registers
    uint32_t get_reg(uint32_t regindex) { return RegisterFile[regindex]; }

    void mem_write(uint16_t address, uint8_t data) {											// Memory write operation
        Memory[address] = data;
    }
	
    uint8_t mem_read(uint16_t address) {														// Memory read operation
        uint8_t data ;
 		data = Memory[address];
        return data;
    }
	
	void print_memory() {																		// Function to Print the memory information in a compact format with register names and values
		for (uint32_t address = 0; address < MEM_SIZE; address++) {
            printf("%x:\t0x%02x\t", address, Memory[address]);
            if ((address+1) % 8 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }

    void print_regs() {																			// Function to Print the register information in a compact format with register names and values
		printf("PC\t:0x%08x\n",pc);
		for (uint32_t index = 0; index < NUM_REGS; index++) {
            printf("%s\t:0x%08x  ", reg_names[index], RegisterFile[index]);
            if ((index + 1) % 4 == 0) {
                printf("\n");
            }
        }
        printf("\n");
	}
	
	
	
	uint32_t LUI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_U31_12=0x%05x,Fields->rd=0x%02x\n",Fields->imm_U31_12,Fields->rd);
		#endif
		RegisterFile[Fields->rd] 	= ((Fields->imm_U31_12) << 12) & 0xfffff000 ; 
		RegisterFile[0] 			= 0x00000000;
		#ifdef DEBUG
			printf("Fields->imm_U31_12=0x%05x,Fields->rd=0x%02x\n",Fields->imm_U31_12,Fields->rd);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t AUIPC(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		#endif
		uint32_t extendimm = (Fields->imm_U31_12) << 12;
		extendimm = extendimm & 0xFFFFF000;
		int32_t signedanswer = static_cast<int32_t>(extendimm);
		RegisterFile[Fields->rd] = pc + signedanswer;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,extendimm=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rd],extendimm,pc);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t JAL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_J20_1=0x%03x,PC=0x%08x,Fields->rd=0x%02x\n", Fields->imm_J20_1, pc, Fields->rd);
		#endif
		int32_t imm_J20_0 			= (Fields->imm_J20_1) << 1;
		int32_t leftshiftimm		= (imm_J20_0) << 11;
		int32_t signed_imm 			= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 		= signed_imm >> 11;
		int32_t effectiveaddress 	= pc + signextendimm;
		if((Fields->rd == 0x01) && (RegisterFile[0x01] == 0x00000000)) {
			cout << "***JAL::END OF SIMULATION*** Encountered Jump RA, and RA = 0" << endl;
			return 0;
		} else {
			RegisterFile[Fields->rd] = pc + 4;
			pc = effectiveaddress - 4;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("PC=0x%08x,RegisterFile[Fields->rd]=0x%08x,signextendimm=0x%08x,effectiveaddress=0x%08x\n", pc, RegisterFile[Fields->rd],signextendimm,effectiveaddress);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t JALR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_I11_0=0x%03x,PC=0x%08x,Fields->rs1=0x%02x,Fields->rd=0x%02x\n", Fields->imm_I11_0, pc, Fields->rs1, Fields->rd);
		#endif
		int32_t leftshiftimm 	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
		int32_t signed_rs1 		= static_cast<int32_t>(RegisterFile[Fields->rs1]);
		int32_t effectiveaddress = signed_rs1 + signextendimm;
		if((Fields->rd == 0x01) && (RegisterFile[0x01] == 0x00000000)) {
			cout << "***JALR::END OF SIMULATION*** Encountered Jump RA, and RA = 0" << endl;
			return 0;
		} else {
			RegisterFile[Fields->rd] = pc + 4;
			pc = (pc + effectiveaddress) - 4;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("PC=0x%08x,RegisterFile[Fields->rs1]=0x%08x, RegisterFile[Fields->rd]=0x%08x,signextendimm=0x%08x,effectiveaddress=0x%08x\n", pc, RegisterFile[Fields->rs1],RegisterFile[Fields->rd], signextendimm, effectiveaddress);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t BEQ(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x PC=%08x \n",Fields->imm_B12_1,Fields->rs2,Fields->rs1,pc);
		#endif
		uint32_t Immediate_Signed;
		if (Fields->imm_B12) 	Immediate_Signed = static_cast<int32_t>(Fields->imm_B12_1 | 0xFFFFE000);
		else 					Immediate_Signed = static_cast<int32_t>(Fields->imm_B12_1);
		Immediate_Signed = Immediate_Signed << 1;
		if (RegisterFile[Fields->rs1] == RegisterFile[Fields->rs2]) pc += Immediate_Signed - 4;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x PC=%08x offset=%08x \n",Fields->imm_B12_1,Fields->rs2,Fields->rs1, pc, Immediate_Signed);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t BNE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_B12_1,Fields->rs1,Fields->rs2);
		#endif
		int32_t signed_rs1			= static_cast<int32_t>(RegisterFile[Fields->rs1]);
		int32_t signed_rs2			= static_cast<int32_t>(RegisterFile[Fields->rs2]);		
		uint32_t signextendoffset 	= Fields->imm_B12_1 >> 11 == 1 ? (Fields->imm_B12_1 << 1 | 0xFFFFE000) : Fields->imm_B12_1 << 1;
		int32_t signedanswer		= static_cast<int32_t>(signextendoffset);
		if(signed_rs1!=signed_rs2) pc += signedanswer -4;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rs1],RegisterFile[Fields->rs2],pc);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t BLT(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x,PC=0x%08x\n", Fields->imm_B12_1, Fields->rs1, Fields->rs2, pc);
		#endif
		int32_t imm_B12_0 		= (Fields->imm_B12_1) << 1;
		int32_t leftshiftimm	= (imm_B12_0) << 19;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 19;
		int32_t signed_rs1 		= static_cast<int32_t>(RegisterFile[Fields->rs1]);
		int32_t signed_rs2 		= static_cast<int32_t>(RegisterFile[Fields->rs2]);
		uint32_t pc_temp 		= pc;
		if (signed_rs1 < signed_rs2) pc = (pc + signextendimm) - 4;
		else    pc = pc;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x,signed_rs1=0x%08x,signed_rs2=0x%08x\n", RegisterFile[Fields->rs1], RegisterFile[Fields->rs2], pc,signed_rs1,signed_rs2);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t BGE(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x,PC=0x%08x\n", Fields->imm_B12_1, Fields->rs1, Fields->rs2, pc);
		#endif
		int32_t imm_B12_0 		= (Fields->imm_B12_1) << 1;
		int32_t leftshiftimm 	= (imm_B12_0) << 19;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 19;
		int32_t signed_rs1 		= static_cast<int32_t>(RegisterFile[Fields->rs1]);
		int32_t signed_rs2 		= static_cast<int32_t>(RegisterFile[Fields->rs2]);
		uint32_t pc_temp 		= pc;
		if (signed_rs1 >= signed_rs2) pc = (pc + signextendimm) - 4;
		else    pc = pc;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x,signed_rs1=0x%08x,signed_rs2=0x%08x, signextendimm = 0x%08x\n", RegisterFile[Fields->rs1], RegisterFile[Fields->rs2], pc, signed_rs1, signed_rs2, signextendimm);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t BLTU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x \n",Fields->imm_B12_1,Fields->rs2,Fields->rs1);
		#endif
		int32_t appendLSB 		= Fields->imm_B12_1 << 1;      		// Appending LSB = 0
		int32_t leftshiftimm	= (appendLSB) << 19; 				// Left shift till MSB(31st position) and also appending LSB=0
        int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
        int32_t rightshiftimm 	= signed_imm >> 19;
		uint32_t rs1 = static_cast<uint32_t>(RegisterFile[Fields->rs1]);
		uint32_t rs2 = static_cast<uint32_t>(RegisterFile[Fields->rs2]);
		if(rs1 < rs2) pc += rightshiftimm - 4;
		else pc = pc;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("PC = %08x \n", pc);
			printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x, PC=%08x, Offset=%08x \n",Fields->imm_B12_1,Fields->rs2,Fields->rs1, pc, rightshiftimm);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t BGEU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG 
			print_regs();
			printf("Fields->imm_B12_1=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_B12_1,Fields->rs1,Fields->rs2);
		#endif
		uint32_t signed_rs1			= RegisterFile[Fields->rs1];
		uint32_t signed_rs2			= RegisterFile[Fields->rs2];	
		uint32_t signextendoffset 	= Fields->imm_B12_1 >>11 == 1 ? (Fields->imm_B12_1 <<1 | 0xFFFFE000) : Fields->imm_B12_1 << 1;
		int32_t  signedanswer		= static_cast<int>(signextendoffset);
		if(signed_rs1 >= signed_rs2) pc+=signedanswer -4;
		else pc = pc;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x,PC=0x%08x\n",RegisterFile[Fields->rs1],RegisterFile[Fields->rs2],pc);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t LB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG 
			print_regs();
			printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		#endif
		int32_t leftshiftimm	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
        uint32_t effaddress 	= RegisterFile[Fields->rs1] + signextendimm;
		uint32_t mem8bit;
		if(effaddress < MEM_SIZE){
			mem8bit 				= Memory[effaddress];
			int32_t leftshiftbyte	= (mem8bit) << 24;
			int32_t signed_byte 	= static_cast<int32_t>(leftshiftbyte);
			int32_t signextendbyte 	= signed_byte >> 24;
			RegisterFile[Fields->rd] = signextendbyte;
		} else {
			cout << "***LB::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effaddress << endl;
			return 0;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x,mem8bit=0x%02x\n",RegisterFile[Fields->rs1], RegisterFile[Fields->rd],effaddress,Memory[effaddress],mem8bit);
			print_regs();
		#endif
		return 1; 
	};
	
	uint32_t LH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->imm_S11_0, Fields->rs1, Fields->rs2);
		#endif
		int32_t leftshiftimm 	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
		uint32_t effaddress 	= RegisterFile[Fields->rs1] + signextendimm;
		uint32_t mem16bit;
		if(effaddress < MEM_SIZE){
			mem16bit 				= Memory[effaddress] | (Memory[effaddress + 1] << 8);
			int32_t leftshift16bit 	= (mem16bit) << 16;
			int32_t signed_16bit 	= static_cast<int32_t>(leftshift16bit);
			int32_t signextend16bit = signed_16bit >> 16;
			RegisterFile[Fields->rd]= signextend16bit;
		} else {
			cout << "***LH::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effaddress << endl;
			return 0;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x,mem16bit=0x%02x\n", RegisterFile[Fields->rs1], RegisterFile[Fields->rd], effaddress, Memory[effaddress], mem16bit);
			print_memory();
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t LW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_B12_1=0x%03x, Fields->rs1=0x%02x, Fields->rd=0x%02x \n",Fields->imm_B12_1, Fields->rs1, Fields->rd);
		#endif
		int Immediate_Signed;
		if(Fields->imm_I11_0 >>11) Immediate_Signed = static_cast<int>(Fields->imm_I11_0 | 0xFFFFF000);
		else Immediate_Signed = static_cast<int>(Fields->imm_I11_0);
		int32_t effectiveaddress = Immediate_Signed + RegisterFile[Fields->rs1];
		if(effectiveaddress < MEM_SIZE){
			RegisterFile[Fields->rd] = Memory[effectiveaddress] | (Memory[effectiveaddress + 1] << 8) | (Memory[effectiveaddress + 2] <<16) | (Memory[effectiveaddress + 3] << 24);
		} else {
			cout << "***LW::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effectiveaddress << endl;
			return 0;
		}	
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("Fields->imm_B12_1=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x, Immediate_Signed=0x%08x, Memory[%08x]=0x%08x \n",Fields->imm_B12_1,Fields->rs2,Fields->rs1, Immediate_Signed, effectiveaddress, Memory[effectiveaddress]);
			print_memory();
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t LBU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG	
			print_regs();
			printf("Fields->imm_I11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_I11_0,Fields->rs1,Fields->rs2);
		#endif
		uint32_t signextendimm 	= Fields->imm_I11_0 >> 11 == 1? (Fields->imm_I11_0) |0xFFFFF000 : Fields->imm_I11_0;
		int32_t signedanswer 	= static_cast<int>(signextendimm);
		int32_t signedrs1 		= static_cast<int>(RegisterFile[Fields->rs1]);
		uint32_t effaddress 	= signedrs1 + signedanswer;
		if(effaddress < MEM_SIZE){
			RegisterFile[Fields->rd] = Memory[effaddress];
		} else {
			cout << "***LBU::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effaddress << endl;
			return 0;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x\n",RegisterFile[Fields->rd],effaddress,Memory[effaddress]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t LHU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		#endif
		int32_t leftshiftimm	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
        uint32_t effaddress 	= RegisterFile[Fields->rs1] + signextendimm;
		uint16_t mem16bit;
		if(effaddress < MEM_SIZE){
			mem16bit = Memory[effaddress] | (Memory[effaddress + 1] << 8);
			RegisterFile[Fields->rd] = static_cast<uint32_t>(mem16bit);
		} else {
			cout << "***LHU::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effaddress << endl;
			return 0;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rd]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x,mem16bit=0x%08x\n",RegisterFile[Fields->rs1], RegisterFile[Fields->rd],effaddress,Memory[effaddress],mem16bit);
			print_regs();
		#endif
		return 1; 
	};
	uint32_t SB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_S11_0=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x \n", Fields->imm_S11_0, Fields->rs2, Fields->rs1);
		#endif
		int32_t Immediate_Signed;
		if (Fields->imm_S11_0 >> 11) Immediate_Signed = static_cast<int>(Fields->imm_S11_0 | 0xFFFFF000);
		else Immediate_Signed = static_cast<int>(Fields->imm_S11_0);
		int32_t effectiveaddress = RegisterFile[Fields->rs1] + Immediate_Signed;
		if(effectiveaddress < MEM_SIZE){
			Memory[effectiveaddress] = RegisterFile[Fields->rs2] & 0x00000FF;
		} else {
			cout << "***SB::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effectiveaddress << endl;
			return 0;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			print_memory();
			printf("Fields->imm_S11_0=0x%03x, Fields->rs2=0x%02x, Fields->rs1=0x%02x, Memory[%08x]=%08x \n", Fields->imm_S11_0, Fields->rs2, Fields->rs1, effectiveaddress, Memory[effectiveaddress]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t SH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG 
			print_regs();
			printf("Fields->imm_S11_0=0x%03x,Fields->rs2=0x%02x,Fields->rs1=0x%02x \n",Fields->imm_S11_0,Fields->rs2,Fields->rs1);
		#endif
		int32_t Immediate_Signed;
		if(Fields->imm_S11_0 >> 11) Immediate_Signed = static_cast<int>(Fields->imm_S11_0 | 0xFFFFF000);
		else Immediate_Signed = static_cast<int>(Fields->imm_S11_0);
		int32_t effectiveaddress 	= RegisterFile[Fields->rs1] + Immediate_Signed;
		if(effectiveaddress < MEM_SIZE){
			Memory[effectiveaddress] 	= RegisterFile[Fields->rs2] & 0x00000FF;
			Memory[effectiveaddress+1]	= (RegisterFile[Fields->rs2] >>8) & 0x000000FF;
		} else {
			cout << "***SH::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effectiveaddress << endl;
			return 0;
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			print_memory();
			printf("Fields->imm_S11_0=0x%03x, Fields->rs2=0x%02x, Fields->rs1=0x%02x, Memory[%08x]=%08x \n",Fields->imm_S11_0,Fields->rs2,Fields->rs1, effectiveaddress, Memory[effectiveaddress]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t SW(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile,map <uint16_t, uint8_t>& Memory){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_S11_0=0x%03x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->imm_S11_0,Fields->rs1,Fields->rs2);
		#endif
		uint32_t signextendimm 	= Fields->imm_S11_0 >> 11 == 1 ? Fields->imm_S11_0 | 0xFFFFF000 : Fields->imm_S11_0;
		int32_t signedanswer 	= static_cast<int32_t>(signextendimm);
		int32_t signedrs1 		= static_cast<int32_t>(RegisterFile[Fields->rs1]);
        uint32_t effaddress 	= signedrs1 + signedanswer;
		if(effaddress < MEM_SIZE){
			Memory[effaddress] 		= RegisterFile[Fields->rs2]       & 0x000000FF;
			Memory[effaddress+1] 	= RegisterFile[Fields->rs2] >> 8  & 0x000000FF;
			Memory[effaddress+2] 	= RegisterFile[Fields->rs2] >> 16 & 0x000000FF;
			Memory[effaddress+3] 	= RegisterFile[Fields->rs2] >> 24 & 0x000000FF;
		} else {
			cout << "***SW::MEMORY ADDRESS OUT OF BOUNDS*** Address Accessed is 0x" << hex << effaddress << endl;
			return 0;
		}
		RegisterFile[0] 		= 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs2]=0x%08x,effaddress=0x%08x,Memory[effaddress]=0x%08x\n",RegisterFile[Fields->rs2],effaddress,Memory[effaddress]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t ADDI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		#endif
		int32_t leftshiftimm	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] + signextendimm;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,signextendimm=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1],signextendimm);
			print_regs();
		#endif
		return 1;
	};
	uint32_t SLTI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("RegisterFile[Fields->rs1]=0x%03x,RegisterFile[Fields->rd]=0x%02x\n",RegisterFile[Fields->rs1],RegisterFile[Fields->rd]);
		#endif
		int32_t leftshiftimm 	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
		RegisterFile[Fields->rd] = (int32_t(RegisterFile[Fields->rs1]) < signextendimm) ? 1 : 0; 
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rs1]=0x%03x,signextendimm=0x%02x,RegisterFile[Fields->rd]=0x%02x\n",RegisterFile[Fields->rs1],signextendimm,RegisterFile[Fields->rd]);
			print_regs();
		#endif
		return 1;
	};
	uint32_t SLTIU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->imm_I11_0=0x%03x,Fields->rs1=0x%02x,Fields->rd=0x%02x",Fields->imm_I11_0,Fields->rs1,Fields->rd);
		#endif
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] < Fields->imm_I11_0 ? 1 : 0;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("Fields->imm_I11_0=0x%03x,Fields->rs1=0x%02x,Fields->rd=0x%02x",Fields->imm_I11_0,Fields->rs1,Fields->rd);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t XORI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm_I11_0=0x%03x\n",Fields->rd,Fields->rs1,Fields->imm_I11_0);
		#endif
		int32_t signextendimm 		= Fields->imm_I11_0 >>11 == 1 ? (Fields->imm_I11_0) | 0xFFFFF000 : Fields->imm_I11_0;
		RegisterFile[Fields->rd] 	= RegisterFile[Fields->rs1] ^ signextendimm;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,signexteneded imm_I11_0=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],signextendimm);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t ORI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm_I11_0=0x%03x\n", Fields->rd, Fields->rs1, Fields->imm_I11_0);
		#endif
		int32_t leftshiftimm	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] | signextendimm;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,signextendimm=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1],signextendimm);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t ANDI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->imm_I11_0=0x%02x\n",Fields->rd,Fields->rs1,Fields->imm_I11_0);
		#endif
		int32_t leftshiftimm	= (Fields->imm_I11_0) << 20;
		int32_t signed_imm 		= static_cast<int32_t>(leftshiftimm);
		int32_t signextendimm 	= signed_imm >> 20;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] & signextendimm; 
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,Fields->imm_I11_0=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],Fields->imm_I11_0);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t SLLI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rs1=0x%02x,Fields->shamt=0x%02x,Fields->rd=0x%02x",Fields->rs1,Fields->shamt,Fields->rd);
		#endif
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] << Fields->shamt;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("Fields->rs1=0x%02x,Fields->shamt=0x%02x,Fields->rd=0x%02x",Fields->rs1,Fields->shamt,Fields->rd);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t SRLI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->shamt=0x%02x\n",Fields->rd,Fields->rs1,Fields->shamt);
		#endif
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] >> Fields->shamt;        
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->shamt]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->shamt]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t SRAI(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->shamt=0x%02x\n", Fields->rd, Fields->rs1, Fields->shamt);
		#endif
		int32_t signed_rs1 = static_cast<int32_t>(RegisterFile[Fields->rs1]);
		RegisterFile[Fields->rd] =  signed_rs1 >> Fields->shamt;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->shamt]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rd]=0x%08x, signed_rs1=0x%08x\n", RegisterFile[Fields->shamt], RegisterFile[Fields->rs1],RegisterFile[Fields->rd], signed_rs1);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t ADD(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG	
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		int32_t signed_rs1 = static_cast<int32_t>(RegisterFile[Fields->rs1]);
		int32_t signed_rs2 = static_cast<int32_t>(RegisterFile[Fields->rs2]);
		RegisterFile[Fields->rd] = signed_rs1 + signed_rs2;        
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t SUB(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		#endif
		RegisterFile[Fields->rd] = static_cast<int32_t>(RegisterFile[Fields->rs1]) - static_cast<int32_t>(RegisterFile[Fields->rs2]);
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd],RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	uint32_t SLL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		uint32_t rs2_shamt = RegisterFile[Fields->rs2] & 0x0000001F;
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] << rs2_shamt;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n, rs2_shamt = 0x%02x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2], rs2_shamt);
			print_regs();
		#endif
		return 1;
	};
	uint32_t SLT(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		int32_t rs1 = int32_t(RegisterFile[Fields->rs1]);
		int32_t rs2 = int32_t(RegisterFile[Fields->rs2]);
		RegisterFile[Fields->rd] = rs1 < rs2 ? 1 : 0;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
			print_regs();
		#endif
		return 1;
	};
	uint32_t SLTU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		if(RegisterFile[Fields->rs1] < RegisterFile[Fields->rs2]) RegisterFile[Fields->rd] = 1;        
		else	RegisterFile[Fields->rd] = 0;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	uint32_t XOR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		#endif
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] ^ RegisterFile[Fields->rs2];
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t SRL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
        #ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		#endif
        uint32_t rs2_shamt = RegisterFile[Fields->rs2] & 0x0000001F;
        RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] >> rs2_shamt;
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n, rs2_shamt = 0x%02x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2], rs2_shamt);
			print_regs();
		#endif
        return 1;
    };
	
	uint32_t SRA(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		int32_t signed_rs1 = static_cast<int32_t>(RegisterFile[Fields->rs1]);
		RegisterFile[Fields->rd] = signed_rs1 >> (RegisterFile[Fields->rs2] & 0x0000001F);
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t OR(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] | RegisterFile[Fields->rs2]; 
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t AND(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] & RegisterFile[Fields->rs2];
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t ECALL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		
		return 1;
	};
	
	uint32_t MUL(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
		#endif
		RegisterFile[Fields->rd] = (RegisterFile[Fields->rs1] * RegisterFile[Fields->rs2]); // places the lower 32 bits result in rd register
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t MULH(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
		#endif
		int64_t rs1 	= static_cast<int64_t>(RegisterFile[Fields->rs1]);
		int64_t rs2 	= static_cast<int64_t>(RegisterFile[Fields->rs2]);
		int64_t result 	= static_cast<int64_t>(rs1) * static_cast<int64_t>(rs2);
        RegisterFile[Fields->rd] = static_cast<int32_t>(result >> 32);
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t MULHSU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		uint64_t unsigned_rs1 = static_cast<uint64_t>(RegisterFile[Fields->rs1]);
		uint64_t unsigned_rs2 = static_cast<uint64_t>(RegisterFile[Fields->rs2]);
		#ifdef DEBUG 
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		uint64_t mult64 = unsigned_rs1 * unsigned_rs2; 
		uint64_t mult64rsh = (mult64 >> 32);
		RegisterFile[Fields->rd]   =  static_cast<uint32_t>(mult64rsh);
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			cout << "value of mult64:" << mult64 << endl;
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t MULHU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG 
			print_regs();
		#endif
		uint64_t unsigned_rs1 	= static_cast<uint64_t>(RegisterFile[Fields->rs1]);
		uint64_t unsigned_rs2 	= static_cast<uint64_t>(RegisterFile[Fields->rs2]);
		int64_t leftshift_rs1	= unsigned_rs1 << 32;
		int64_t signedtemp_rs1 	= static_cast<int64_t>(leftshift_rs1);
		int64_t signextend_rs1 	= signedtemp_rs1 >> 32;
		int64_t signed_rs1 		= static_cast <int64_t>(signextend_rs1);
		uint64_t mult64 		= signed_rs1 * unsigned_rs2; 
		uint64_t mult64rsh 		= (mult64 >> 32);
		RegisterFile[Fields->rd]   =  static_cast<uint32_t>(mult64rsh);
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			cout << "value of mult64:" << mult64 << endl;
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t DIV(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		int32_t signed_rs1 = static_cast<int32_t>(RegisterFile[Fields->rs1]);
		int32_t signed_rs2 = static_cast<int32_t>(RegisterFile[Fields->rs2]);
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		if (signed_rs1==0x80000000 && signed_rs2==0xffffffff) RegisterFile[Fields->rd] = 0x80000000;
		else 		RegisterFile[Fields->rd] = signed_rs2==0 ? 0xFFFFFFFF : signed_rs1 / signed_rs2;        
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t DIVU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		uint32_t usigned_rs1 = RegisterFile[Fields->rs1];
		uint32_t usigned_rs2 = RegisterFile[Fields->rs2];
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x,Fields->rs1=0x%02x,Fields->rs2=0x%02x\n",Fields->rd,Fields->rs1,Fields->rs2);
		#endif
		RegisterFile[Fields->rd] = usigned_rs2==0 ? 0xFFFFFFFF : usigned_rs1 / usigned_rs2;         
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x,RegisterFile[Fields->rs1]=0x%08x,RegisterFile[Fields->rs2]=0x%08x\n",RegisterFile[Fields->rd],RegisterFile[Fields->rs1],RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
	
	uint32_t REM(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		int32_t signed_rs1 = static_cast<int32_t>(RegisterFile[Fields->rs1]);
		int32_t signed_rs2 = static_cast<int32_t>(RegisterFile[Fields->rs2]);
		#ifdef DEBUG 
			print_regs();
			printf("Fields->rd=0x%02x, Fields->rs1=0x%02x, Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		#endif
		if (signed_rs2 == 0) { 															// division by zero
			RegisterFile[Fields->rd] = signed_rs1; 										// remainder is equal to dividend
		}
		else { 																			// valid division
			RegisterFile[Fields->rd] = signed_rs1 % signed_rs2; 						// compute remainder
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x, RegisterFile[Fields->rs1]=0x%08x, RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1; 
	};
	
	uint32_t REMU(struct InstrFields *Fields,map<uint32_t, uint32_t>& RegisterFile){
		#ifdef DEBUG
			print_regs();
			printf("Fields->rd=0x%02x, Fields->rs1=0x%02x, Fields->rs2=0x%02x\n", Fields->rd, Fields->rs1, Fields->rs2);
		#endif
		if (RegisterFile[Fields->rs2] == 0) {  													// division by zero
			RegisterFile[Fields->rd] = RegisterFile[Fields->rs1]; 								// remainder is equal to dividend
		}
		else { 																					// valid division
			RegisterFile[Fields->rd] = RegisterFile[Fields->rs1] % RegisterFile[Fields->rs2]; 	// compute remainder
		}
		RegisterFile[0] = 0x00000000;
		#ifdef DEBUG
			printf("RegisterFile[Fields->rd]=0x%08x, RegisterFile[Fields->rs1]=0x%08x, RegisterFile[Fields->rs2]=0x%08x\n", RegisterFile[Fields->rd], RegisterFile[Fields->rs1], RegisterFile[Fields->rs2]);
			print_regs();
		#endif
		return 1;
	};
