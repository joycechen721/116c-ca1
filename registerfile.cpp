#include <cstdint>
#include <array>
#include "registerfile.h"

RegisterFile::RegisterFile() {
    // initialize all registers to 0
    for (int i = 0; i < 32; i++) {
        registers[i] = 0;
    }
}

// read from register
uint32_t RegisterFile::read(int reg_num) const {
    if (reg_num >= 32) {
        return 0;
    }
    // x0 is always 0 in RISC-V
    if (reg_num == 0) {
        return 0;
    }
    return registers[reg_num];
}

// write to register
void RegisterFile::write(int reg_num, uint32_t value) {
    if (reg_num >= 32 || reg_num == 0) {
        // cannot write to x0 or invalid registers
        return;
    }
    registers[reg_num] = value;
}

// reset all registers
void RegisterFile::reset() {
    for (int i = 0; i < 32; i++) {
        registers[i] = 0;
    }
}