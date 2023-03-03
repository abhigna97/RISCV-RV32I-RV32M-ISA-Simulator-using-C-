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
	//execute a loop if file opened successfully
	while(myfile){
		myfile>>PCValue>>Instruction;
		InstructionDecode(Instruction,PCValue);
	}

	myfile.close();
    
	return 0;
}
