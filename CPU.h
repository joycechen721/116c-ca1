#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include "control.h"
#include "registerfile.h"
using namespace std;


// class instruction { // optional
// public:
// 	bitset<32> instr;//instruction
// 	instruction(bitset<32> fetch); // constructor

// };

class CPU {
	private:
		int dmemory[4096]; //data memory byte addressable in little endian fashion;
		unsigned long PC; //pc 
		ControlUnit control; // controller
		RegisterFile regFile; // register file

	public:
		CPU();
		unsigned long readPC();
		void incPC();
		int decode(std::bitset<32> instruction);
};
