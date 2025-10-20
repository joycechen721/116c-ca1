#ifndef ALU_H
#define ALU_H
#include <iostream>
#include <bitset>
#include "control.h"

class ALU {
    private:
        // ALU operations
        enum Operation {
            ADD = 0,
            SUB = 1,
            AND = 2,
            OR = 3,
            SLT = 4,  // Set Less Than
            SLL = 5,  // Shift Left Logical
            SRA = 6   // Shift Right Arithmetic
        };

        // signals
        int ALUOp;
        uint32_t Input1;
        uint32_t Input2;
        uint32_t ALUResult;
        ControlUnit* control; // pointer to control unit

        Operation getALUOp(int funct3) const;

    public:
        ALU(int ALUOp = 0, ControlUnit* ctrl = nullptr);

        void initialize(int ALUOp, int funct3, uint32_t input1, uint32_t input2);

        uint32_t execute();
        
        uint32_t getResult() const { return ALUResult; }
};
#endif // ALU_H