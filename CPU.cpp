#include "CPU.h"


CPU::CPU()
{
	memory = Memory();
	// initialize controller and datapath components
	control = ControlUnit();
	regFile = RegisterFile(&control);
	ALUUnit = ALUController(0, &control);

	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		memory.writeByte(i, 0);
	}
}

unsigned long CPU::readPC()
{
	return PC;
}

void CPU::incPC(int32_t increment)
{
	PC += increment;
}

void CPU::setPC(unsigned long newPC)
{
	PC = newPC;
}

// Add other functions here ... 

// decode instruction and send values to datapath components and controller.
int CPU::decodeInstruction(bitset<32> instruction)
{
	// extract opcode (bits 0-6) for controller
	bitset<7> opcode(instruction.to_ulong() & 0x7F);
	OpCode op = static_cast<OpCode>(opcode.to_ulong());
	this->control.setControlSignals(op);

	// initialize reg file
	regFile.setReadReg1((instruction.to_ulong() >> 15) & 0x1F); // rs1
	regFile.setReadReg2((instruction.to_ulong() >> 20) & 0x1F); // rs2
	regFile.setWriteReg((instruction.to_ulong() >> 7) & 0x1F); // rd

	// generate immediate value
	int32_t imm = ImmGen::generateImmediate(instruction);

	// execute ALU
	int32_t input1 = two_bit_mux(regFile.getReadData1(), static_cast<int32_t>(0), control.getLUI());
	int32_t input2 = two_bit_mux(regFile.getReadData2(), imm, control.getALUSrc());
	input2 = two_bit_mux(input2, static_cast<int32_t>(0), control.getLUI());
	bitset<7> funct3((instruction.to_ulong() >> 12) & 0x7);
	uint32_t funct3_val = funct3.to_ulong();
	ALUUnit.execute(funct3_val, input1, input2);

	// handle memory operations
	int32_t readData = 0;
	int32_t numBytes = 0;

	// determine byte size and perform memory operations
	int32_t address = ALUUnit.getResult();
	
	// map funct3 to number of bytes: 0/4->1, 1/5->2, 2->4
	if (funct3_val <= 2) {
		numBytes = 1 << funct3_val; // 1, 2, or 4 bytes
	} else if (funct3_val == 4 || funct3_val == 5) {
		numBytes = 1 << (funct3_val - 4); // 1 or 2 bytes (unsigned loads)
	}

	if (control.getMemRead()) { 
		if (numBytes == 1) readData = static_cast<int32_t>(memory.readByte(address));
		else if (numBytes == 2) readData = static_cast<int32_t>(memory.readHalfWord(address));
		else if (numBytes == 4) readData = static_cast<int32_t>(memory.readWord(address));
	} else if (control.getMemWrite()) {
		int32_t data = regFile.getReadData2();
		if (numBytes == 1) memory.writeByte(address, static_cast<int8_t>(data & 0xFF));
		else if (numBytes == 2) memory.writeHalfWord(address, static_cast<int16_t>(data & 0xFFFF));
		else if (numBytes == 4) memory.writeWord(address, data);
	}

	// write memory or ALU result back to register file
	int32_t writeData = 0;
	if (control.getRegWrite()) {
		writeData = two_bit_mux(address, readData, control.getMemToReg());
		writeData = two_bit_mux(writeData, static_cast<int32_t>((PC + 1) * 4), control.getJump());
		writeData = two_bit_mux(writeData, imm, control.getLUI());
		regFile.setWriteData(writeData);
	}

	// update PC
	if (control.getJump()) {
		int32_t target_byte_addr = address & ~1;
		int32_t target_pc = target_byte_addr / 4;
		setPC(target_pc);
		return 0;
	}
	int32_t increment = two_bit_mux(static_cast<int32_t>(4), imm, control.getBranch() && !ALUUnit.getZero());
	incPC(increment / 4);

	return 0;
}

int CPU::getRegisterOutputValue(int regNum) const { 
	return regFile.read(regNum); 
}