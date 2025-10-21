#include "memory.h"

Memory::Memory(uint32_t size) : memory(size, 0) {}

uint8_t Memory::readByte(uint32_t address) const {
    return memory[address];
}

void Memory::writeByte(uint32_t address, int8_t value) {
    memory[address] = value;
}

int32_t Memory::readWord(uint32_t address) const {
    return *reinterpret_cast<const int32_t*>(&memory[address]);
}

void Memory::writeWord(uint32_t address, int32_t value) {
    *reinterpret_cast<int32_t*>(&memory[address]) = value;
}

int16_t Memory::readHalfWord(uint32_t address) const {
    return *reinterpret_cast<const int16_t*>(&memory[address]);
}

void Memory::writeHalfWord(uint32_t address, int16_t value) {
    *reinterpret_cast<int16_t*>(&memory[address]) = value;
}
