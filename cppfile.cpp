// https://www.geeksforgeeks.org/file-handling-c-classes/ 
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

map<int, int> DataMemory;
map<int, int> RegisterFile;

// stack<int> stack;
int array[int(argv[0])-1:0];

// global variable - to keep track of stacktop

using namespace std;

int pushstack(int a){
array[0]=a;
}

int ADDfunction(int rs1, int rs2){
return rs1+rs2;
}

InstructionDecode(int Instruction, int PCValue){
	int rs1, rs2, rd;

    switch(Instruction[6:0]): {// funct3 and funct7 of any two instruction can also at times represent I-field
		case 7'b0110111: // LUI
			break;
		case 7'b0010111: // AUIPC
			break;
		case 7'b1101111: // JAL
			break;
		case 7'b1100111: // JALR
			break;
		case 7'b1100011: // BEQ, BNE, BLT, BGE, BLTU, BGEU
		{
			switch(Instruction[14:12]):
				case 3'b000: // BEQ
				break;
				case 3'b001: // BNE
				break;
				case 3'b100: // BLT
				break;
				case 3'b101: // BGE
				break;
				case 3'b110: // BLTU
				break;
				case 3'b111: // BGEU
				break;
		}
		case 7'b0000011: // LB, LH, LW, LBU, LHU
		{
			switch(Instruction[14:12]):
				case 3'b000:
				break;
				case 3'b001:
				break;
				case 3'b010:
				break;
				case 3'b100:
				break;
				case 3'b101:
				break;
		}
		case 7'b0100011: // SB, SH, SW
		{
			switch(Instruction[14:12])
				case 3'b000:
				break;
				case 3'b001:
				break;
				case 3'b010:
				break;
		}
		case 7'b0010011: // ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI
		{
	    	switch(Instruction[14:12]): {
				case 3'b000:
				break;
				case 3'b010:
				break;
				case 3'b011:
				break;
				case 3'b100:
				break;
				case 3'b110:
				break;
				case 3'b111:
				break;
				case 3'b001: // SLLI
				break;
				case 3'b101:
					if(Instruction[31:25]==7'b0000000){
						// SRLI
					} else {
						// SRAI
					}
				break;
			}
		}
		case 7'b0110011:{ // ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND 
			switch(Instruction[14:12]):{
				case 3'b000:
					if(Instruction[31:25]==7'b0000000){ // ADD
						rs1=RegisterFile[Instruction[19:15]];
						cout<<"Pushing rs1 to stack"<<endl;
						pushstack(rs1);
						rs2=RegisterFile[Instruction[24:20]];
						cout<<"Pushing rs2 to stack"<<endl;
						pushstack(rs2);
						// by now all of the fields in the Instruction have been taken care of.
						// calling function ADD to run/execute the Instruction
						cout<<"Exiting Decode Stage"<<endl;
						// passing values onto the stack
						
						
						rd = ADDfunction(PCValue, rs1, rs2);
						RegisterFile[Instruction[11:7]] = rd;
					} else if(Instruction[31:25]==7'b0100000) { // SUB

					}
					else
					cout<<"Invalid Instruction\n";
				break;
				case 3'b001:
				break;
				case 3'b010:
				break;
				case 3'b011:
				break;
				case 3'b100:
				break;
				case 3'b101:{
					if(Instruction[31:25]==7'b0000000){

					} else {

					}
				}
				break;
				case 3'b110:

				break;
				case 3'b111:
				break;
			
			}
		}
		break;
		case 7'b0001111: // FENCE 

		break;
		case 7'b1110011: // ECALL, EBREAK 
			if(Instruction[31:20] == 12'b000000000000){

			} else {
				
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

	//execute a loop if file opened successfully
	while(myfile){
		myfile>>PCValue>>Instruction;
		InstructionDecode(Instruction,PCValue);
	}

	myfile.close();
    
	return 0;
}
