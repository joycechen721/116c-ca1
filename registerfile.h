#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <cstdint>

class RegisterFile {
private:
    uint32_t registers[32];

public:
    RegisterFile();
    
    // read register value
    uint32_t read(int regNum) const;
    
    // write to register (register 0 is hardwired to 0)
    void write(int regNum, uint32_t value);
    
    // reset all registers to 0
    void reset();
    
    // print register contents (for debugging)
    void print() const;
};

#endif // REGISTERFILE_H