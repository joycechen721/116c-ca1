#include "CPU.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include<fstream>
#include <sstream>
using namespace std;

/*
Add all the required standard and developed libraries here
*/

/*
Put/Define any helper function/definitions you need here
*/
int main(int argc, char* argv[])
{
	/* This is the front end of your project.
	You need to first read the instructions that are stored in a file and load them into an instruction memory.
	*/

	/* Each cell should store 1 byte. You can define the memory either dynamically, or define it as a fixed size with size 4KB (i.e., 4096 lines). Each instruction is 32 bits (i.e., 4 lines, saved in little-endian mode).
	Each line in the input file is stored as an hex and is 1 byte (each four lines are one instruction). You need to read the file line by line and store it into the memory. You may need a mechanism to convert these values to bits so that you can read opcodes, operands, etc.
	*/

	// check for file argument
	if (argc < 2) {
		cout << "No file name entered. Exiting...";
		return -1;
	}

	// open the file
	ifstream infile(argv[1]);
	if (!(infile.is_open() && infile.good())) {
		cout<<"error opening file\n";
		return 0; 
	}

	// load the instructions line by line (each line is a byte)
	const size_t INSTMEM_SIZE = 4096;
	array<uint8_t, INSTMEM_SIZE> instMem;
	size_t bytes_loaded = 0;
	string line;
	while (infile >> line) {
		instMem[bytes_loaded++] = std::stoul(line, nullptr, 16);
	}

	// max number of instructions
	int maxPC = bytes_loaded / 4;

	/* Instantiate your CPU object here.  CPU class is the main class in this project that defines different components of the processor.
	CPU class also has different functions for each stage (e.g., fetching an instruction, decoding, etc.).
	*/

	CPU myCPU;  // call the approriate constructor here to initialize the processor...  
	// make sure to create a variable for PC and resets it to zero (e.g., unsigned int PC = 0); 
	
	bool done = true;
	while (done == true) // processor's main loop. Each iteration is equal to one clock cycle.  
	{
		//fetch instruction
		unsigned int pc = myCPU.readPC();
		if (pc >= maxPC) break;

		// convert 4 bytes into a 32-bit instruction (little-endian)
		uint32_t instruction = instMem[pc * 4] | 
							   (instMem[pc * 4 + 1] << 8) | 
							   (instMem[pc * 4 + 2] << 16) | 
							   (instMem[pc * 4 + 3] << 24);
		bitset<32> instrBits(instruction);
		cout << "Fetched instruction at PC " << pc << ": 0x" << hex << instruction << " (binary: " << instrBits << ")" << dec << "\n";

		// decode

		// increment the PC and break once instructions are done executing
		myCPU.incPC();
		if (myCPU.readPC() > maxPC)
			break;
	}

	// int a0 =0;
	// int a1 =0;  
	// print the results (you should replace a0 and a1 with your own variables that point to a0 and a1)
	// cout << "(" << a0 << "," << a1 << ")" << endl;
	
	return 0;
}