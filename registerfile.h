#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <cstdint>
#include "control.h"

class RegisterFile {
    private:
        uint32_t registers[32];
        int ReadReg1;
        int ReadReg2;
        int WriteReg;
        ControlUnit *control; // pointer to control unit

    public:
        RegisterFile(ControlUnit* ctrl = nullptr);
        
        // read register value
        uint32_t read(int regNum) const;
        
        // write to register (register 0 is hardwired to 0)
        void write(int regNum, uint32_t value);
        
        // set read registers
        void setReadReg1(int regNum);
        void setReadReg2(int regNum);
        
        // set write register
        void setWriteReg(int regNum);

        // set write data
        void setWriteData(uint32_t data);
        
        // get read register values
        uint32_t getReadData1() const;
        uint32_t getReadData2() const;
        
        // reset all registers to 0
        void reset();
        
        // print register contents (for debugging)
        void print() const;
};

#endif // REGISTERFILE_H