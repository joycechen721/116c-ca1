#include <iostream>
#include <bitset>
#include "control.h"

// decode instruction opcode and set control signals
void ControlUnit::decode(std::bitset<7> opcode) {
    // reset all signals
    RegWrite = MemRead = MemWrite = MemToReg = ALUSrc = Branch = Jump = false;
    ALUOp = 0;
    
    unsigned int op = opcode.to_ulong();
    
    switch(op) {
        case 0x33: // R-type (ADD, SUB, AND, OR, etc.)
            RegWrite = true;
            ALUOp = 2;
            break;
            
        case 0x13: // I-type arithmetic (ADDI, SLTI, etc.)
            RegWrite = true;
            ALUSrc = true;
            ALUOp = 2;
            break;
            
        case 0x03: // Load (LW, LH, LB, etc.)
            RegWrite = true;
            MemRead = true;
            MemToReg = true;
            ALUSrc = true;
            ALUOp = 0;
            break;
            
        case 0x23: // Store (SW, SH, SB, etc.)
            MemWrite = true;
            ALUSrc = true;
            ALUOp = 0;
            break;
            
        case 0x63: // Branch (BEQ, BNE, BLT, BGE, etc.)
            Branch = true;
            ALUOp = 1;
            break;
            
        case 0x6F: // JAL
            RegWrite = true;
            Jump = true;
            break;
            
        case 0x67: // JALR
            RegWrite = true;
            Jump = true;
            ALUSrc = true;
            break;
            
        default:
            std::cerr << "Unknown opcode: 0x" << std::hex << op << std::endl;
            break;
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
int ControlUnit::getALUOp() const { return ALUOp; }

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
    std::cout << "  ALUOp:    " << ALUOp << "\n";
}