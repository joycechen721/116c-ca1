#ifndef IMMGEN_H
#define IMMGEN_H

#include <bitset>
#include "isa.h"
#include <iostream>

class ImmGen {
public:
    // Generate immediate value based on instruction type
    static int32_t generateImmediate(std::bitset<32> instruction) {
        int32_t imm = 0;
        std::bitset<7> opcode(instruction.to_ulong() & 0x7F);
        OpCode op = static_cast<OpCode>(opcode.to_ulong());

        switch(op) {
            case J_TYPE: // JALR is an I-type instruction
            case I_TYPE_LOAD:
            case I_TYPE:
                // extract immediate (bits 20-31, sign-extended)
                imm = (instruction.to_ulong() >> 20) & 0xFFF;
                if (imm & 0x800) imm |= 0xFFFFF000; // sign extend
                break;

            case S_TYPE:
                // extract immediate (bits 7-11, 25-31, sign-extended)
                imm = ((instruction.to_ulong() >> 7) & 0x1F) | ((instruction.to_ulong() >> 20) & 0xFE0);
                if (imm & 0x800) imm |= 0xFFFFF000; // sign extend
                break;

            case B_TYPE:
                // extract immediate (bits 8-11, 25-30, 7, 31, sign-extended)
                imm = ((instruction.to_ulong() >> 7) & 0x1E) | ((instruction.to_ulong() >> 20) & 0x7E0) |
                      ((instruction.to_ulong() << 4) & 0x800) | ((instruction.to_ulong() >> 19) & 0x1000);
                if (imm & 0x1000) imm |= 0xFFFFE000; // sign extend
                break;

            case U_TYPE:
                // extract immediate (bits 12-31)
                imm = instruction.to_ulong() & 0xFFFFF000;
                break;
                
            default:
                imm = 0;
                break;
        }
        
        return imm;
    }
};

#endif // IMMGEN_H