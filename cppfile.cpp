// https://www.geeksforgeeks.org/file-handling-c-classes/ 
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;
map<int, int> DataMemory;       // using only for Loads and Stores
map<int, int> RegisterFile;     // fetching register values

// global variable - to keep track of stacktop
int sp;
int StackSize;

// stack<int> stack;
int stackDS[StackSize-1:0];    // stack DataStructure - used as a stack





int pushstack(int a){
array[sp]=a;
}

int popstack(void){
return array[sp];
}

int ADDfunction(int rs1, int rs2){
return rs1+rs2;
}

int SUBfunction(int rs1, int rs2){
return rs1-rs2;
}

InstructionDecode(int Instruction, int PCValue){
	int rs1, rs2, rd;

    switch(Instruction[6:0]): {									// funct3 and funct7 of any two instruction can also at times represent I-field
		case 0b0110111: // LUI
			break;
		case 0b0010111: // AUIPC
			break;
		case 0b1101111: // JAL
			break;
		case 0b1100111: // JALR
			break;
		case 0b1100011: // BEQ, BNE, BLT, BGE, BLTU, BGEU
		{
			switch(Instruction[14:12]):
				case 0b000: // BEQ
				break;
				case 0b001: // BNE
				break;
				case 0b100: // BLT
				break;
				case 0b101: // BGE
				break;
				case 0b110: // BLTU
				break;
				case 0b111: // BGEU
				break;
		}
		case 0b0000011: // LB, LH, LW, LBU, LHU
		{
			switch(Instruction[14:12]):
				case 0b000:
				break;
				case 0b001:
				break;
				case 0b010:
				break;
				case 0b100:
				break;
				case 0b101:
				break;
		}
		case 0b0100011: // SB, SH, SW
		{
			switch(Instruction[14:12])
				case 0b000:
				break;
				case 0b001:
				break;
				case 0b010:
				break;
		}
		case 0b0010011: // ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI
		{
	    	switch(Instruction[14:12]): {
				case 0b000:
				break;
				case 0b010:
				break;
				case 0b011:
				break;
				case 0b100:
				break;
				case 0b110:
				break;
				case 0b111:
				break;
				case 0b001: // SLLI
				break;
				case 0b101:
					if(Instruction[31:25]= 0b0000000){
						// SRLI
					} else {
						// SRAI
					}
				break;
			}
		}
		case 0b0110011:{ // ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND 
			switch(Instruction[14:12]):{
				case 0b000:
					if(Instruction[31:25]= 0b0000000){ // ADD
						rs1=RegisterFile[Instruction[19:15]];
						rs2=RegisterFile[Instruction[24:20]];
						// by now all of the fields in the Instruction have been taken care of.
						// calling function ADD to run/execute the Instruction
						cout<<"Exiting Decode Stage"<<endl;
						// passing values onto the stack		
						rd = ADDfunction(PCValue, rs1, rs2);
						RegisterFile[Instruction[11:7]] = rd;
					} else if(Instruction[31:25]= 0b0100000) { // SUB
						rs1=RegisterFile[Instruction[19:15]];
						rs2=RegisterFile[Instruction[24:20]];
						// by now all of the fields in the Instruction have been taken care of.
						// calling function ADD to run/execute the Instruction
						cout<<"Exiting Decode Stage"<<endl;
						// passing values onto the stack		
						rd = SUBfunction(PCValue, rs1, rs2);
						RegisterFile[Instruction[11:7]] = rd;
					}
					else
					cout<<"Invalid Instruction\n";
				break;
				case 0b001: // SLL

				break;
				case 0b010: // SLT
				break;
				case 0b011: // SLTU
				break;
				case 0b100: // XOR
				break;
				case 0b101:{ 
					if(Instruction[31:25]= 0b0000000){ // SRL

					} else if(Instruction[31:25]= 0b0100000){ // SRA

					} else {
						cout<<"Invalid Instruction"<<endl;
					}
				}
				break;
				case 0b110:   // OR

				break;
				case 0b111:   // AND
				break;
			
			}
		}
		break;
		case 0b0001111: // FENCE 

		break;
		case 0b1110011: // ECALL 
			if(Instruction[31:20] ==  0b000000000000){

			} else if(Instruction[31:20] ==  0b000000000001){  // EBREAK
				
			} else {
				cout<<"Invalid Instruction"<<endl;
			}
		break;
		default:
	}
}

int main(int argc, char** argv)
{
	ifstream myfile(prog.mem);

	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";
    
    StackSize = int(argv[1]); // argv[0] is the program name
    PCValue = int(argv[2]);
	
		myfile>>PCValue>>Instruction;
		InstructionDecode(Instruction,PCValue);
	}

	myfile.close();
    
	return 0;
}







////////////////////////////////////////////////

struct Instruction{ //Struct type for Instruction that holds different part of the Instruction
    unsigned int opcode;         //opcode
    unsigned int rs1;            //rs1- source register1
    unsigned int rs2;            //rs2- source register2
    unsigned int rd;             //rd - destination register
    unsigned int funct3;        //Funct 3 
    unsigned int funct7;        //funct 7
    unsigned int imm_I;          //Immediate value for I Instruction format
    unsigned int shamt;          //Shamt for R type format
    unsigned int shamt_I;        //Shamt for I type format 
    unsigned int imm_S1;         //Imm[4:0] for S type format
    unsigned int imm_S2;         //Imm[11:5] for S type format
    unsigned int imm_S3;         //Imm[11:0] for S type format
    unsigned int imm_B; 
    unsigned int imm_J;
    unsigned int imm_JALR;
    unsigned int imm_LUI;
    unsigned int imm_AUIPC;
    unsigned int target;
};


/////Register Decode for R type//////


void Register_Decode_Rtype(struct Instruction *instr, int inputinstrn)
{
int tempinstrn; // temporary variable to store the Instruction from file
tempinstrn = inputinstrn;  //  assigning the temporary variable with the Instruction
tempinstrn = tempinstrn>>7; //shifting tempinstrn by 6 positions to get RD
instr->rd =  tempinstrn & 0x0000001F; // masking tempinstrn with mask value to get value of RD
   // printf("rd = %d\n",instr->rd);
    tempinstrn  = inputinstrn; // assigning  Instruction again to temporary variable
    tempinstrn = tempinstrn>>15; //shifting tempinstrn by 15 positions to get rs1
    instr->rs1 = tempinstrn & 0x0000001F; // masking tempinstrn with a mask value to get value of rs1
   // printf("rs1 = %d\n",instr->rs1);
    tempinstrn  = inputinstrn;; // assigning  Instruction again to temporary variable
    tempinstrn = tempinstrn>>15; //shifting tempinstrn by 20 positions to get rs2
    instr->rs2 = tempinstrn & 0x0000001F; // masking tempinstrn with a mask value to get value of rs2
   // printf("rs2 = %d\n",instr->rs2);
   tempinstrn  = inputinstrn;; // assigning  Instruction again to temporary variable
    tempinstrn = tempinstrn>>12; //shifting tempinstrn by 12 positions to get funct3
    instr->funct3 = tempinstrn & 0x00000007; // masking tempinstrn with a mask value to get value of funct3
   //printf("%x\n",instr->funct3);
   tempinstrn  = inputinstrn; // assigning  Instruction again to temporary variable
    tempinstrn = tempinstrn>>25; //shifting tempinstrn by 25 positions to get funct7
    instr->funct7 = tempinstrn & 0X0000007F; // masking tempinstrn with a mask value to get value of funct7
   //printf("%x\n",instr->funct7);
}

////////InstructionDecode for R type/////////

void Rtype_Instruction(struct Instruction *instr, int inputinstrn, int temp_arr[]){
   {
            if(instr->funct7 == 0) //add operation
            Register_Decode_Rtype(instr,inputinstrn); // calling the function to seperate register fields
            temp_arr[instr->rd] = (int)temp_arr[instr->rs1]+(int)temp_arr[instr->rs2]; //add operation
           // printf("%d\n",temp_arr[instr->rd]);
         }
         else if(instr->funct7 == 32){ //Subtract operation
         {
            Register_Decode_Rtype(instr,inputinstrn); // calling the function to seperate register fields
            temp_arr[instr->rd] = (int)temp_arr[instr->rs1] - (int)temp_arr[instr->rs2]; //subtract operation
           // printf("%d",temp_arr[instr->rd]);
            }
         }




