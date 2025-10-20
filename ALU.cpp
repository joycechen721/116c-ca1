#include "ALU.h"

ALU::ALU(int ALUOp, ControlUnit* ctrl) : Input1(0), Input2(0), ALUResult(0), ALUOp(ALUOp), control(ctrl) {}

void ALU::initialize(int ALUOp, int funct3, uint32_t input1, uint32_t input2) {
    this->ALUOp = getALUOp(funct3);
    this->Input1 = input1;
    this->Input2 = input2;
}

ALU::Operation ALU::getALUOp(int funct3) const {
    switch(control->getALUOp()) {
        case ALU_ADD: // Load/Store/Jump (LBU, LW, SH, SW, JALR)
            return ADD;

        case ALU_SUB: // Branches (BNE)
            return SUB;

        case ALU_R_TYPE: // R-type (SRA, SUB, AND)
            switch(funct3) {
                case 0: return SRA;
                case 5: return SUB;
                case 7: return AND;
            }

        case ALU_I_TYPE: // I-type (LUI, ADDI, ORI, SLTIU)
            switch(funct3) {
                case 0: return ADD;
                case 3: return SLT;
                case 6: return OR;
            }
        
        case ALU_LUI: // LUI
            return SLL;

        default:
            return ADD;
    }
}

uint32_t ALU::execute() {
    switch(ALUOp) {
        case ADD:
            ALUResult = Input1 + Input2;
            break;
        case SUB:
            ALUResult = Input1 - Input2;
            break;
        case AND:
            ALUResult = Input1 & Input2;
            break;
        case OR:
            ALUResult = Input1 | Input2;
            break;
        case SLT:
            ALUResult = (static_cast<int32_t>(Input1) < static_cast<int32_t>(Input2)) ? 1 : 0;
            break;
        case SLL:
            ALUResult = Input1 << (Input2 & 0x1F);
            break;
        case SRA:
            ALUResult = static_cast<int32_t>(Input1) >> (Input2 & 0x1F);
            break;
    }
    return ALUResult;
}