#include "CPU.h"


CPU::CPU()
{
	memory = Memory();
	// initialize controller and datapath components
	control = ControlUnit();
	regFile = RegisterFile(&control);
	ALUUnit = ALU(0, &control);

	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		memory.writeByte(i, 0);
	}
}

unsigned long CPU::readPC()
{
	// cout << "Current PC: " << PC << endl;
	return PC;
}

void CPU::incPC(int32_t increment)
{
	// cout << "Incrementing PC by " << increment << endl;
	PC += increment;
}

void CPU::setPC(unsigned long newPC)
{
	// cout << "Setting PC to " << newPC << endl;
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
	// std::cout << "Opcode: " << opcode << std::endl;
	// this->control.printControlSignals();

	// initialize reg file
	regFile.setReadReg1((instruction.to_ulong() >> 15) & 0x1F); // rs1
	regFile.setReadReg2((instruction.to_ulong() >> 20) & 0x1F); // rs2
	regFile.setWriteReg((instruction.to_ulong() >> 7) & 0x1F); // rd

	// generate immediate value
	int32_t imm = ImmGen::generateImmediate(instruction);
	// cout << "Generated immediate: " << imm << std::endl;

	// execute ALU
	int32_t input1 = two_bit_mux(regFile.getReadData1(), static_cast<int32_t>(0), control.getLUI());
	int32_t input2 = two_bit_mux(regFile.getReadData2(), imm, control.getALUSrc());
	input2 = two_bit_mux(input2, static_cast<int32_t>(0), control.getLUI());
	bitset<7> funct3((instruction.to_ulong() >> 12) & 0x7);
	uint32_t funct3_val = funct3.to_ulong();
	ALUUnit.initialize(control.getALUOp(), funct3_val, input1, input2);
	ALUUnit.execute();
	// cout << "ALU result: " << ALUUnit.getResult() << std::endl;

	// handle memory operations
	int32_t readData = 0;
	int32_t numBytes = 0;

	// determine byte size based on funct3
	if (funct3_val == 0 || funct3_val == 4) { // LB/SB or LBU
		numBytes = 1;
	} else if (funct3_val == 1 || funct3_val == 5) { // LH/SH or LHU
		numBytes = 2;
	} else if (funct3_val == 2) { // LW/SW
		numBytes = 4;
	}

	int32_t address = ALUUnit.getResult();

	if (control.getMemRead()) { 
		// cout << "Loading " << numBytes << " bytes from memory address " << address << ": 0x" << hex << memory.readWord(address) << dec << std::endl;
		// load from memory (little-endian)
		if (numBytes == 1) {
			readData = static_cast<int32_t>(memory.readByte(address));
		} else if (numBytes == 2) {
			readData = static_cast<int32_t>(memory.readHalfWord(address));
		} else if (numBytes == 4) {
			readData = static_cast<int32_t>(memory.readWord(address));
		}
		
		// 0 extend for LBU and LHU
		if (funct3_val == 4) { // LBU
			readData &= 0xFF;
		} else if (funct3_val == 5) { // LHU
			readData &= 0xFFFF;
		}
	} else if (control.getMemWrite()) {
		// cout << "Storing " << numBytes << " bytes to memory address " << address << ": 0x" << hex << (regFile.getReadData2() & ((1ULL << (numBytes * 8)) - 1)) << dec << std::endl;
		// store to memory (little-endian)
		if (numBytes == 1) {
			memory.writeByte(address, static_cast<int8_t>(regFile.getReadData2() & 0xFF));
		} else if (numBytes == 2) {
			memory.writeHalfWord(address, static_cast<int16_t>(regFile.getReadData2() & 0xFFFF));
		} else if (numBytes == 4) {
			memory.writeWord(address, static_cast<int32_t>(regFile.getReadData2()));
		}
	}

	// write memory or ALU result back to register file
	int32_t writeData = 0;
	if (control.getRegWrite()) {
		writeData = two_bit_mux(address, readData, control.getMemToReg());
		writeData = two_bit_mux(writeData, static_cast<int32_t>((PC + 1) * 4), control.getJump());
		writeData = two_bit_mux(writeData, imm, control.getLUI());
		regFile.setWriteData(writeData);
	}

	// print destination register value
	// cout << "Wrote to register x" << ((instruction.to_ulong() >> 7) & 0x1F) << ": " << writeData << std::endl;

	// update PC
	if (control.getJump()) {
		int32_t target_byte_addr = address & ~1;
		int32_t target_pc = target_byte_addr / 4;
		// cout << "Jumping to byte address " << target_byte_addr << " (PC=" << target_pc << ")" << endl;
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