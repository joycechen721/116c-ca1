#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>
#include <bitset>
#include "isa.h"
using namespace std;

class CPU;

class ControlUnit {
    private:
        bool RegWrite, MemRead, MemWrite, MemToReg, ALUSrc, Branch, Jump;
        int ALUOp, Size;
        
    public:
        ControlUnit();
        
        // decode instruction opcode and set control signals
        void setControlSignals(OpCode opcode, int funct3 = 0);

        // getters for control signals
        bool getRegWrite() const;
        bool getMemRead() const;
        bool getMemWrite() const;
        bool getMemToReg() const;
        bool getALUSrc() const;
        bool getBranch() const;
        bool getJump() const;
        int getALUOp() const;
        int getSize() const;

        // print current control signals
        void printControlSignals() const;
};

#endif // CONTROL_H