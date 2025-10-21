#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <vector>

class Memory {
    private:
        std::vector<uint8_t> memory;

    public:
        Memory(uint32_t size = 1048576); // default size 1MB

        // LB, SB
        uint8_t readByte(uint32_t address) const;
        void writeByte(uint32_t address, int8_t value);

        // LW, SW
        int32_t readWord(uint32_t address) const;
        void writeWord(uint32_t address, int32_t value);

        // LH, SH
        int16_t readHalfWord(uint32_t address) const;
        void writeHalfWord(uint32_t address, int16_t value);
};

#endif // MEMORY_H