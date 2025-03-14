#ifndef CPU_H
#define CPU_H

#define inst_ref(func) std::bind(&func, this, std::placeholders::_1)

constexpr Byte
    JSR = 0x20,
    LDA_I = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    LDA_ABS = 0x6D,
    LDA_ABSX = 0xBD,
    LDA_INDX = 0xA1,
    LDA_INDY = 0xB1,
    LDX_I = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ABS = 0xAE,
    RTS = 0x60;

class CPU {
private:
    // registers
    Word PC;
    Byte SP, A, X, Y;

    // processor status register
    bool C, Z, I, D, B, O, N;

    std::unordered_map<Byte, std::function<void(int&)>> instructions = {
        {JSR, inst_ref(jumpSubRoutine)},
        {LDA_I, inst_ref(loadAImmediate)},
        {LDA_ZP, inst_ref(loadAZeroPage)},
        {LDA_ZPX, inst_ref(loadAZeroPageX)},
        {LDA_ABS, inst_ref(loadAAbsolute)},
        {LDA_ABSX, inst_ref(loadAAbsoluteX)},
        {LDA_INDX, inst_ref(loadAIndexedIndirect)},
        {LDA_INDY, inst_ref(loadAIndirectIndexed)},
        {LDX_I, inst_ref(loadXImmediate)},
        {LDX_ZP, inst_ref(loadXZeroPage)},
        {LDX_ABS, inst_ref(loadXAbsolute)},
        {RTS, inst_ref(returnFromSubRoutine)},
    };

    Memory& mem;

public:
    CPU(Memory& memory): mem(memory) {}

    void reset() {
        PC = 0xFFFC;
        SP = 0xFF;
        A = X = Y = 0;
        C = Z = I = D = B = O = N = 0;
        mem.reset();
    }

    Byte fetchByte(int& cycles) {
        Byte b = mem[PC];
        PC++;
        cycles--;

        return b;
    }

    Word fetchWord(int& cycles) {
        Word w = mem[PC];
        PC++;
        w |= (mem[PC] << 8);
        PC++;
        cycles -= 2;

        return w;
    }

    Byte readByte(int& cycles, Word address) {
        Byte b = mem[address];
        cycles--;

        return b;
    }

    Word readWord(int& cycles, Word address) {
        Word w = readByte(cycles, address);
        w |= (readByte(cycles, address + 1) << 8);

        return w;
    }

    void writeWord(int& cycles, Word w, Word address) {
        mem[address] = w & 0x00FF; // mask convert word to byte
        mem[address + 1] = (w >> 8);
        cycles -= 2;
    }

    void setLoadFlags(Byte reg) {
        Z = (reg == 0);
        N = ((reg & 0x80) == 0x80);
    }

    void execute(int cycles) {
        while (cycles > 0) {
            Byte opcode = fetchByte(cycles);
            if (instructions.count(opcode))
                instructions[opcode](cycles);
        }
    }

    // jump to subroutine
    void jumpSubRoutine(int& cycles);

    // load A
    void loadAImmediate(int& cycles);
    void loadAZeroPage(int& cycles);
    void loadAZeroPageX(int& cycles);
    void loadAAbsolute(int& cycles);
    void loadAAbsoluteX(int& cycles);
    void loadAAbsoluteY(int& cycles);
    void loadAIndexedIndirect(int& cycles);
    void loadAIndirectIndexed(int& cycles);

    // load X
    void loadXImmediate(int& cycles);
    void loadXZeroPage(int& cycles);
    void loadXAbsolute(int& cycles);

    // return from subroutine
    void returnFromSubRoutine(int& cycles);

    Byte getA() { return A; }
    Byte getX() { return X; }
    Byte getY() { return Y; }
};

#endif
