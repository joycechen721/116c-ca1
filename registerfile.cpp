#include <cstdint>
#include <array>
#include "registerfile.h"

RegisterFile::RegisterFile(ControlUnit* ctrl) : control(ctrl) {
    // initialize all registers to 0
    for (int i = 0; i < 32; i++) {
        registers[i] = 0;
    }
    control = ctrl;
}

// read from register
int32_t RegisterFile::read(int reg_num) const {
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
void RegisterFile::write(int reg_num, int32_t value) {
    if (reg_num >= 32 || reg_num == 0) {
        // cannot write to x0 or invalid registers
        return;
    }
    registers[reg_num] = value;
}

// set read registers
void RegisterFile::setReadReg1(int regNum) {
    ReadReg1 = regNum;
}

void RegisterFile::setReadReg2(int regNum) {
    ReadReg2 = regNum;
}

// set write register
void RegisterFile::setWriteReg(int regNum) {
    WriteReg = regNum;
}

// set write data
void RegisterFile::setWriteData(int32_t data) {
    if (WriteReg > 0 && WriteReg < 32) {
        registers[WriteReg] = data;
    }
}

// get read register values
int32_t RegisterFile::getReadData1() const {
    if (ReadReg1 == 0) {
        return 0;
    }
    return registers[ReadReg1];
}

int32_t RegisterFile::getReadData2() const {
    if (ReadReg2 == 0) {
        return 0;
    }
    return registers[ReadReg2];
}

// reset all registers
void RegisterFile::reset() {
    for (int i = 0; i < 32; i++) {
        registers[i] = 0;
    }
}