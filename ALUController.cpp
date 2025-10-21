#include "ALUController.h"

ALUController::ALUController(int ALUOp, ControlUnit* ctrl) : Input1(0), Input2(0), ALUResult(0), control(ctrl) {}

ALUController::ArithmeticOperation ALUController::getArithmeticOp(int funct3) const {
    switch(control->getALUOp()) {
        case ALU_ADD: // Load/Store/Jump (LBU, LW, SH, SW, JALR)
            return ADD;

        case ALU_SUB: // Branches (BNE)
            return SUB;

        case ALU_R_TYPE: // R-type (SRA, SUB, AND)
            switch(funct3) {
                case 0: return SUB;
                case 5: return SRA;
                case 7: return AND;
            }

        case ALU_I_TYPE: // I-type (ADDI, ORI, SLTIU)
            switch(funct3) {
                case 0: return ADD;
                case 3: return SLT;
                case 6: return OR;
            }

        default:
            return ADD;
    }
}

void ALUController::execute(int funct3, int32_t Input1, int32_t Input2) {
    int ArithmeticOp = getArithmeticOp(funct3);
    switch(ArithmeticOp) {
        case ADD:
            // cout << "Performing ADD operation: " << Input1 << " + " << Input2 << endl;
            this->ALUResult = Input1 + Input2;
            break;
        case SUB:
            // cout << "Performing SUB operation: " << Input1 << " - " << Input2 << endl;  
            this->ALUResult = Input1 - Input2;
            break;
        case AND:
            // cout << "Performing AND operation: " << Input1 << " & " << Input2 << endl;
            this->ALUResult = Input1 & Input2;
            break;
        case OR:
            // cout << "Performing OR operation: " << Input1 << " | " << Input2 << endl;
            this->ALUResult = Input1 | Input2;
            break;
        case SLT:
            // cout << "Performing SLT operation: " << Input1 << " < " << Input2 << endl;
            this->ALUResult = (static_cast<int32_t>(Input1) < static_cast<int32_t>(Input2)) ? 1 : 0;
            break;
        case SRA:
            // cout << "Performing SRA operation: " << Input1 << " >> " << (Input2 & 0x1F) << endl;
            this->ALUResult = static_cast<int32_t>(Input1) >> (Input2 & 0x1F);
            break;
    }
}