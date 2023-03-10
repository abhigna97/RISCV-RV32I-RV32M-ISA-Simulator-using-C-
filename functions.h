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
uint32_t ADD(struct InstrFields *Fields){
	//temp_arr[instr->rd] = (int)temp_arr[instr->rs1]+(int)temp_arr[instr->rs2];
	RegisterFile[Fields->rd] = RegisterFile[Fields->rs1]+ RegisterFile[Fields->rs2];
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