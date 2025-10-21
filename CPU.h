#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include "isa.h"
#include "control.h"
#include "registerfile.h"
#include "ALUController.h"
#include "immgen.h"
#include "memory.h"
using namespace std;


class Instruction {
public:
    std::bitset<32> instr; // 32-bit instruction
    explicit Instruction(const std::bitset<32>& fetch) : instr(fetch) {} // contructor
};
	
class CPU {
	private:
		// int dmemory[4096]; //data memory byte addressable in little endian fashion;
		Memory memory; // data memory
		unsigned long PC; //pc 
		ControlUnit control; // controller
		RegisterFile regFile; // register file
		ALUController ALUUnit; // ALU

	public:
		CPU();
		unsigned long readPC();
		void incPC(int32_t increment);
		void setPC(unsigned long newPC);
		int decodeInstruction(std::bitset<32> instruction);
		int getRegisterOutputValue(int regNum) const;
};

#endif // CPU_H