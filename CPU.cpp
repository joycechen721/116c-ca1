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
	return PC;
}
void CPU::incPC()
{
	PC++;
}

// Add other functions here ... 

// decode instruction and send values to datapath components and controller.
int CPU::decodeInstruction(std::bitset<32> instruction)
{
	// extract opcode (bits 0-6) for controller
	std::bitset<7> opcode(instruction.to_ulong() & 0x7F);
	OpCode op = static_cast<OpCode>(opcode.to_ulong());
	this->control.setControlSignals(op);
	std::cout << "Opcode: " << opcode << std::endl;
	this->control.printControlSignals();

	// handle different instruction types
	switch(op) {
		case R_TYPE: {
			// initialize regfile
			regFile.setReadReg1((instruction.to_ulong() >> 15) & 0x1F); // rs1
			regFile.setReadReg2((instruction.to_ulong() >> 20) & 0x1F); // rs2
			regFile.setWriteReg((instruction.to_ulong() >> 7) & 0x1F); // rd

			// initialize ALU
			std::bitset<3> funct3((instruction.to_ulong() >> 12) & 0x7);
			ALUUnit.initialize(control.getALUOp(), funct3.to_ulong(),
			                      regFile.getReadData1(),
			                      regFile.getReadData2());

			// execute ALU operation and store result
			ALUUnit.execute();
			std::cout << "R-type instruction. ALU Result: " << ALUUnit.getResult() << std::endl;
			break;
		}
		case LOAD:
		case I_TYPE: {
			// extract immediate (bits 20-31, sign-extended)
			long imm = (instruction.to_ulong() >> 20) & 0xFFF;
			if (imm & 0x800) imm |= 0xFFFFF000; // sign extend

			// initialize regfile
			regFile.setReadReg1((instruction.to_ulong() >> 15) & 0x1F); // rs1
			regFile.setWriteReg((instruction.to_ulong() >> 7) & 0x1F); // rd

			// initialize ALU
			std::bitset<3> funct3((instruction.to_ulong() >> 12) & 0x7);
			ALUUnit.initialize(control.getALUOp(), funct3.to_ulong(),
			                    regFile.getReadData1(),
			                    imm);

			std::cout << "I-type instruction. imm: " << imm << std::endl;
			break;
		}
		case S_TYPE: {
			// extract immediate (bits 7-11, 25-31, sign-extended)
			long imm = ((instruction.to_ulong() >> 7) & 0x1F) | ((instruction.to_ulong() >> 20) & 0xFE0);
			if (imm & 0x800) imm |= 0xFFFFF000; // sign extend

			// initialize regfile
			regFile.setReadReg1((instruction.to_ulong() >> 15) & 0x1F); // rs1
			regFile.setReadReg2((instruction.to_ulong() >> 20) & 0x1F); // rs2

			// initialize ALU
			std::bitset<3> funct3((instruction.to_ulong() >> 12) & 0x7);
			ALUUnit.initialize(control.getALUOp(), funct3.to_ulong(),
			                    regFile.getReadData1(),
			                    imm);

			std::cout << "S-type instruction. funct3: " << funct3 << ", imm: " << imm << std::endl;
			break;
		}
		case B_TYPE: {
			// extract immediate (bits 8-11, 25-30, 7, 31, sign-extended)
			long imm = ((instruction.to_ulong() >> 7) & 0x1E) | ((instruction.to_ulong() >> 20) & 0x7E0) |
					   ((instruction.to_ulong() << 4) & 0x800) | ((instruction.to_ulong() >> 19) & 0x1000);
			if (imm & 0x1000) imm |= 0xFFFFE000; // sign extend

			// initialize regfile
			regFile.setReadReg1((instruction.to_ulong() >> 15) & 0x1F); // rs1
			regFile.setReadReg2((instruction.to_ulong() >> 20) & 0x1F); // rs2

			// initialize ALU
			std::bitset<3> funct3((instruction.to_ulong() >> 12) & 0x7);
			ALUUnit.initialize(control.getALUOp(), funct3.to_ulong(),
			                    regFile.getReadData1(),
			                    regFile.getReadData2());

			std::cout << "B-type instruction. funct3: " << funct3 << ", imm: " << imm << std::endl;
			break;
		}
		case LUI: {
			// extract immediate (bits 12-31)
			long imm = instruction.to_ulong() & 0xFFFFF000;

			// initialize regfile
			regFile.setWriteReg((instruction.to_ulong() >> 7) & 0x1F); // rd

			// initialize ALU
			ALUUnit.initialize(control.getALUOp(), 0, 0, imm);
			std::cout << "LUI instruction. imm: " << imm << std::endl;
			break;
		}
		case JUMP: {
			// extract immediate (bits 21-30, 20, 12-19, 31, sign-extended)
			long imm = ((instruction.to_ulong() >> 20) & 0x7FE) | ((instruction.to_ulong() >> 9) & 0x800) |
					   (instruction.to_ulong() & 0xFF000) | ((instruction.to_ulong() >> 11) & 0x100000);
			if (imm & 0x100000) imm |= 0xFFE00000; // sign extend

			// initialize regfile
			regFile.setWriteReg((instruction.to_ulong() >> 7) & 0x1F); // rd

			// initialize ALU
			ALUUnit.initialize(control.getALUOp(), 0, regFile.getReadData1(), imm);
			std::cout << "JUMP instruction. imm: " << imm << std::endl;
			break;
		}
		default: {
			std::cout << "Instruction type not handled in decodeInstruction." << std::endl;
			break;
		}
	}
	
	return 0;
}