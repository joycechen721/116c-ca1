#include "CPU.h"


CPU::CPU()
{
	// initialize controller and datapath components
	control = ControlUnit();
	regFile = RegisterFile(&control);
	ALUUnit = ALU(0, &control);

	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}
}

unsigned long CPU::readPC()
{
	cout << "Current PC: " << PC << endl;
	return PC;
}

void CPU::incPC(int32_t increment)
{
	cout << "Incrementing PC by " << increment << endl;
	PC += increment;
}

// Add other functions here ... 

// decode instruction and send values to datapath components and controller.
int CPU::decodeInstruction(bitset<32> instruction)
{
	// extract opcode (bits 0-6) for controller
	bitset<7> opcode(instruction.to_ulong() & 0x7F);
	OpCode op = static_cast<OpCode>(opcode.to_ulong());
	this->control.setControlSignals(op);
	// std::cout << "Opcode: " << opcode << std::endl;
	// this->control.printControlSignals();

	// initialize reg file
	regFile.setReadReg1((instruction.to_ulong() >> 15) & 0x1F); // rs1
	regFile.setReadReg2((instruction.to_ulong() >> 20) & 0x1F); // rs2
	regFile.setWriteReg((instruction.to_ulong() >> 7) & 0x1F); // rd

	// generate immediate value
	int32_t imm = ImmGen::generateImmediate(instruction);
	cout << "Generated immediate: " << imm << std::endl;

	// initialize ALU
	int32_t input1 = two_bit_mux(regFile.getReadData1(), static_cast<int32_t>(0), control.getLUI());
	int32_t input2 = two_bit_mux(regFile.getReadData2(), imm, control.getALUSrc());
	input2 = two_bit_mux(input2, static_cast<int32_t>(0), control.getLUI());
	bitset<7> funct3((instruction.to_ulong() >> 12) & 0x7);
	ALUUnit.initialize(control.getALUOp(), funct3.to_ulong(), input1, input2);
	ALUUnit.execute();
	cout << "ALU result: " << ALUUnit.getResult() << std::endl;

	// handle memory operations
	int32_t readData = 0;
	int32_t address = ALUUnit.getResult();
	if (control.getMemRead()) { // load from memory (little-endian), store to output register
		for (int i = 0; i < control.getSize(); i++) {
			readData |= (dmemory[address + i] << (i * 8));
		}
	} else if (control.getMemWrite()) {	// store a register value to memory (little-endian)
		for (int i = 0; i < control.getSize(); i++) {
			dmemory[address + i] = (regFile.getReadData2() >> (i * 8)) & 0xFF;
		}
	}

	// write memory or ALU result back to register file
	int32_t writeData = 0;
	if (control.getRegWrite()) {
		writeData = two_bit_mux(address, readData, control.getMemToReg());
		writeData = two_bit_mux(writeData, static_cast<int32_t>(PC + 1), control.getJump());
		writeData = two_bit_mux(writeData, imm, control.getLUI());
		regFile.setWriteData(writeData);
	}

	// print destination register value
	cout << "Wrote to register x" << ((instruction.to_ulong() >> 7) & 0x1F) << ": " << writeData << std::endl;

	// update PC
	int32_t increment = two_bit_mux(static_cast<int32_t>(1), imm, control.getBranch() && !ALUUnit.getZero());
	increment = two_bit_mux(increment, static_cast<int32_t>(address - PC), control.getJump());
	incPC(increment);

	return 0;
}