#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <bits/stdc++.h>


using namespace std;

map<int, int> Memory;       			// Modelling the Memory
map<string,uint32_t>RegisterFile;		// Modeling the 32 RISCV General Purpose Registers

const char *reg_names[] = {
            "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
            "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
            "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
            "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

const uint32_t MEM_SIZE = 65536;  									// 64 KB
const uint32_t WORD_SIZE = 8;    									// 8 bytes
const uint32_t NUM_REGS = sizeof(reg_names) / sizeof(reg_names[0]);	// Calculate the number of registers: 32 in RISC-V

class RISCV {
	
private:
    uint32_t pc;						// Modeling Program counter
   

public:
    
    void set_pc(uint32_t pcvalue) { pc = pcvalue; }											// Getters and setters for the program counter
    uint32_t get_pc() const { return pc; }

    void set_reg(string regname, uint32_t regvalue) { RegisterFile[regname] = regvalue; }	// Getters and setters for general-purpose registers
    uint32_t get_reg(string regname) const { return RegisterFile[regname]; }

    void mem_write(uint32_t address, uint32_t data) {										// Memory read operation
        for (uint32_t i = 0; i < WORD_SIZE; i++) {
            Memory[address + i] = (data >> (i * 8)) & 0xffffffff;
        }
    }
    uint32_t mem_read(uint32_t address) const {												// Memory write operation
        uint32_t data = 0;
        for (uint32_t i = 0; i < WORD_SIZE; i++) {
            data |= ((uint32_t) Memory[address + i]) << (i * 8);
        }
        return data;
    }
	
	void initialize(){																		// initialize PC and all GPRs with a given value
		pc = 0;
		for(uint32_t i=0;i<NUM_REGS;i++){										
	    RegisterFile[reg_names[i]] = 0;}
	}

    void print_regs() const {																// Function to Print the register information in a compact format with register names and values
        printf("PC: 0x%08x\n", pc);
        for (uint32_t i = 0; i < NUM_REGS; i++) {
            printf("%s:\t0x%08x ", reg_names[i], RegisterFile[reg_names[i]]);
            if ((i + 1) % 4 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }
	
	void print_memory() const {																// Function to Print the memory information in a compact format with register names and values
        for (uint32_t i = 0; i < MEM_SIZE; i=i+1) {
            printf("%x: 0x%08x ", i, Memory[i]);
            if ((i + 1) % 8 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }
	
};



int main(int argc, char *argv[]) {
    RISCV cpu;   			 			// Create a RISC-V CPU instance
    uint32_t pc,sp;
	cpu.set_pc(0x0);					// initialize pc,sp,ra to 0
    RegisterFile["sp"] = 0;
    RegisterFile["ra"] = 0;
    string memoryimage = "program.mem";

    if (argc >= 4) {
        cerr << "*****Incorrect Number of Arguments Provided. Should be Used as follows: \n" << argv[0] << " <pc> <sp> <memoryimage>" << endl;
        return EXIT_FAILURE;
    } else {

    // Parse PC and SP as 32-bit hexadecimal values
    if(argv[1]){
		stringstream ss_pc(argv[1]);
		ss_pc >> hex >> pc;
	} else {
		

    stringstream ss_sp(argv[2]);
    ss_sp >> hex >> sp;

    memoryimage = argv[3];
	}

    // printing the parsed arguments
    cout << "PC = 0x" << setfill('0') << setw(8) << hex << pc << endl;
    cout << "SP = 0x" << setfill('0') << setw(8) << hex << sp << endl;
    cout << "Memory image filename = " << memoryimage << endl;
	
	/*for(uint32_t i=0;i<NUM_REGS;i++){					// initialize all GPRs with a given value
	    RegisterFile[reg_names[i]] = 0;
	}*/
	ifstream memory_file(memoryimage);
	int address,data;
	if (memory_file.is_open()) {
        
        /*while (memory_file >> std::hex >> address >> data) {
            Memory[address] = data;
        }*/
		std::string line;

        while (std::getline(memory_file, line)) {
            std::stringstream ss(line);
            int address, data;

            ss >> std::hex >> address;
            ss.ignore(2, ' '); // Ignore the colon and space between address and data
            ss >> std::hex >> data;

            Memory[address] = data;
        }

        memory_file.close();
    } else {
        std::cerr << "Failed to open memory image file." << std::endl;
        return 1;
    }
   
	cpu.set_pc(0x1000);
	cpu.set_reg("t0",0x12345678);
	cpu.get_reg("t0");
    cpu.initialize();
    //cpu.mem_write(0xfffe, 0x12345678);					// Write some data to memory
    //cpu.mem_write(0xffbc, 0xdeadbeef);
    cpu.print_regs();
	cpu.print_memory();

    return EXIT_SUCCESS;

}

// commands to use: g++ -o chk1 chkpt1.cpp ; ./chk1 98 76 program.mem
