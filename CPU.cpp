#include "CPU.h"


CPU::CPU()
{
	// initialize controller and datapath components
	control = ControlUnit();
	regFile = RegisterFile();

	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}
}


unsigned long CPU::readPC()
{
	return PC;
}
void CPU::incPC()
{
	PC++;
}

// Add other functions here ... 
int CPU::decode(std::bitset<32> instruction)
{
	// extract opcode (bits 0-6) for controller
	std::bitset<7> opcode(instruction.to_ulong() & 0x7F);
	this->control.decode(opcode);
	std::cout << "Opcode: " << opcode << std::endl;
	
	return 0;
}