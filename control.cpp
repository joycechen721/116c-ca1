#include <iostream>
#include <bitset>
#include "control.h"

// constructor
ControlUnit::ControlUnit() : RegWrite(false), MemRead(false), MemWrite(false),
                              MemToReg(false), ALUSrc(false), Branch(false),
                              Jump(false), LUI(false), ALUOp(0), Size(0) {}

// decode instruction opcode and set control signals
void ControlUnit::setControlSignals(OpCode opcode, int funct3) {
    // reset all signals
    ALUOp = ALU_ADD;
    Size = 0;
    RegWrite = MemRead = MemWrite = MemToReg = ALUSrc = Branch = Jump = LUI = false;

    switch(opcode) {
        case R_TYPE: { // R-type (SRA, SUB, AND)
            ALUOp = ALU_R_TYPE;
            RegWrite = true;
            break;
        }

        case U_TYPE: { // LUI
            ALUOp = ALU_ADD;
            RegWrite = LUI = true;
            break;
        }

        case I_TYPE: { // I-type (ADDI, ORI, SLTIU)
            ALUOp = ALU_I_TYPE;
            RegWrite = ALUSrc = true;
            break;
        }

        case I_TYPE_LOAD: { // Load (LBU, LW)
            ALUOp = ALU_ADD;
            RegWrite = ALUSrc = MemRead = MemToReg = true;
            if (funct3 == 0x4) { // LBU
                Size = 1;
            }
            else if (funct3 == 0x2) { // LW
                Size = 4;
            }
            break;
        }

        case S_TYPE: { // Store (SH, SW)
            ALUOp = ALU_ADD;
            ALUSrc = MemWrite = true;
            if (funct3 == 0x1) { // SH
                Size = 2;
            }
            else if (funct3 == 0x2) { // SW
                Size = 4;
            }
            break;
        }

        case B_TYPE: { // Branch (BNE)
            ALUOp = ALU_SUB;
            Branch = true;
            break;
        }

        case J_TYPE: { // Jump (JALR)
            ALUOp = ALU_ADD;
            RegWrite = ALUSrc = Jump = true;
            break;
        }

        default: {
            std::cerr << "Unknown opcode: 0x" << std::hex << opcode << std::endl;
            break;
        }
    }
}

// getters for control signals
bool ControlUnit::getRegWrite() const { return RegWrite; }
bool ControlUnit::getMemRead() const { return MemRead; }
bool ControlUnit::getMemWrite() const { return MemWrite; }
bool ControlUnit::getMemToReg() const { return MemToReg; }
bool ControlUnit::getALUSrc() const { return ALUSrc; }
bool ControlUnit::getBranch() const { return Branch; }
bool ControlUnit::getJump() const { return Jump; }
bool ControlUnit::getLUI() const { return LUI; }
int ControlUnit::getALUOp() const { return ALUOp; }
int ControlUnit::getSize() const { return Size; }

// print current control signals
void ControlUnit::printControlSignals() const {
    std::cout << "Control Signals:\n";
    std::cout << "  RegWrite: " << RegWrite << "\n";
    std::cout << "  MemRead:  " << MemRead << "\n";
    std::cout << "  MemWrite: " << MemWrite << "\n";
    std::cout << "  MemToReg: " << MemToReg << "\n";
    std::cout << "  ALUSrc:   " << ALUSrc << "\n";
    std::cout << "  Branch:   " << Branch << "\n";
    std::cout << "  Jump:     " << Jump << "\n";
    std::cout << "  LUI:      " << LUI << "\n";
    std::cout << "  ALUOp:    " << ALUOp << "\n";
    std::cout << "  Size:     " << Size << "\n";
}