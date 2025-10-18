#include <iostream>
#include <bitset>

class ControlUnit {
private:
    // control signals
    bool RegWrite;
    bool MemRead;
    bool MemWrite;
    bool MemToReg;
    bool ALUSrc;
    bool Branch;
    bool Jump;
    int ALUOp;
    
public:
    ControlUnit() : RegWrite(false), MemRead(false), MemWrite(false),
                    MemToReg(false), ALUSrc(false), Branch(false),
                    Jump(false), ALUOp(0) {}
    
    // decode instruction opcode and set control signals
    void decode(std::bitset<7> opcode);
    
    // getters for control signals
    bool getRegWrite() const;
    bool getMemRead() const;
    bool getMemWrite() const;
    bool getMemToReg() const;
    bool getALUSrc() const;
    bool getBranch() const;
    bool getJump() const;
    int getALUOp() const;

    // print current control signals
    void printControlSignals() const;
};