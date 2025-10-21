#ifndef ALU_H
#define ALU_H
#include <iostream>
#include <bitset>
#include "control.h"

class ALUController {
    private:
        // ALU operations
        enum ArithmeticOperation {
            ADD = 0,
            SUB = 1,
            AND = 2,
            OR = 3,
            SLT = 4,  // Set Less Than
            SRA = 5   // Shift Right Arithmetic
        };

        // signals
        int32_t Input1;
        int32_t Input2;
        int32_t ALUResult;
        ControlUnit* control; // pointer to control unit

        ArithmeticOperation getArithmeticOp(int funct3) const;

    public:
        ALUController(int ALUOp = 0, ControlUnit* ctrl = nullptr);

        void initialize(int ALUOp, int funct3, int32_t input1, int32_t input2);

        void execute(int funct3, int32_t Input1, int32_t Input2);
        
        int32_t getResult() const { return ALUResult; }

        bool getZero() const { return ALUResult == 0; }
};
#endif // ALU_H