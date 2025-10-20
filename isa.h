#ifndef ISA_H
#define ISA_H

// the opcode types
enum OpCode {
    R_TYPE = 0x33,
    I_TYPE = 0x13,
    LOAD = 0x03,
    S_TYPE = 0x23,
    LUI = 0x37,
    B_TYPE = 0x63,
    JUMP = 0x67
};

// ALU Operation Signals
enum ALUOp {
    ALU_ADD = 0, // load/store/jump (LBU, LW, SH, SW, JALR)
    ALU_SUB = 1, // branches (BNE)
    ALU_R_TYPE = 2, // R-type (SRA, SUB, AND)
    ALU_I_TYPE = 3,  // I-type (LUI, ADDI, ORI, SLTIU)
    ALU_LUI = 4  // LUI
};

// 2-to-1 multiplexer
inline int two_bit_mux(unsigned int a, unsigned int b, bool sel) {
    return sel ? b : a;
}

#endif // ISA_H