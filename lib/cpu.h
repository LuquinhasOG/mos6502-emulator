#ifndef CPU_H
#define CPU_H

#define bnd(func) std::bind(&func, this)

constexpr Word
    PAGE_ZERO = 0x0000,
    PAGE_ONE = 0x0100,
    STACK_BOTTOM = 0x01FF;

// instructions
constexpr Byte
    JSR = 0x20,
    LDA_I = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    LDA_ABS = 0x6D,
    LDA_ABSX = 0xBD,
    LDA_ABSY = 0xB9,
    LDA_INDX = 0xA1,
    LDA_INDY = 0xB1,
    LDX_I = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ZPY = 0xB6,
    LDX_ABS = 0xAE,
    LDX_ABSY = 0xBE,
    LDY_I = 0xA0,
    LDY_ZP = 0xA4,
    LDY_ZPX = 0xB4,
    LDY_ABS = 0xAC,
    LDY_ABSX = 0xBC,
    STX_ZP = 0x86,
    STX_ZPY = 0x96,
    STX_ABS = 0x8E,
    RTS = 0x60;

// illegal instructions (for debug)
constexpr Byte
    JAM = 0x02; // kill the cpu execution

class CPU {
private:
    // registers
    Word PC;
    Byte SP, A, X, Y;

    // processor status register (K is the kill flag, debug feature)
    bool C, Z, I, D, B, O, N, K;

    std::unordered_map<Byte, std::function<void(void)>> instructions = {
        {JSR, bnd(jumpSubRoutine)},
        {LDA_I, bnd(loadAImmediate)},
        {LDA_ZP, bnd(loadAZeroPage)},
        {LDA_ZPX, bnd(loadAZeroPageX)},
        {LDA_ABS, bnd(loadAAbsolute)},
        {LDA_ABSX, bnd(loadAAbsoluteX)},
        {LDA_ABSY, bnd(loadAAbsoluteY)},
        {LDA_INDX, bnd(loadAIndexedIndirect)},
        {LDA_INDY, bnd(loadAIndirectIndexed)},
        {LDX_I, bnd(loadXImmediate)},
        {LDX_ZP, bnd(loadXZeroPage)},
        {LDX_ZPY, bnd(loadXZeroPageY)},
        {LDX_ABS, bnd(loadXAbsolute)},
        {LDX_ABSY, bnd(loadXAbsoluteY)},
        {LDY_I, bnd(loadYImmediate)},
        {LDY_ZP, bnd(loadYZeroPage)},
        {LDY_ZPX, bnd(loadYZeroPageX)},
        {LDY_ABS, bnd(loadYAbsolute)},
        {LDY_ABSX, bnd(loadYAbsoluteX)},
        {STX_ZP, bnd(storeXZeroPage)},
        {STX_ZPY, bnd(storeXZeroPageY)},
        {STX_ABS, bnd(storeXAbsolute)},
        {RTS, bnd(returnFromSubRoutine)},
        // illegal instructions
        {JAM, bnd(kill)},
    };

    Memory& mem;
    unsigned long cycles; // count cycles in CPU

public:
    CPU(Memory& memory): mem(memory) {}

    void reset() {
        cycles = 0;
        PC = 0xFFFC;
        SP = 0xFF;
        A = X = Y = 0;
        C = Z = I = D = B = O = N = K = 0;
        mem.reset();
    }

    Byte fetchByte() {
        Byte b = mem[PC];
        PC++;

        return b;
    }

    Word fetchWord() {
        Word w = mem[PC];
        PC++;
        w |= (mem[PC] << 8);
        PC++;

        return w;
    }

    Byte readByteZeroPage(Byte reg = 0) {
        return mem[(PAGE_ZERO | fetchByte()) + reg];
    }

    Byte readByteAbsolute(Byte reg = 0) {
        Word address = fetchWord() + reg;
        if ((address & 0xFF00) != (address+X & 0xFF00))
            cycles++;
        return mem[address];
    }

    Word readWord(Word address) {
        return mem[address] | (mem[address + 1] << 8);;
    }

    void writeWord(Word w, Word address) {
        mem[address] = w & 0x00FF; // mask convert word to byte
        mem[address + 1] = (w >> 8);
    }

    void setLoadFlags(Byte reg) {
        Z = (reg == 0);
        N = ((reg & 0x80) == 0x80);
    }

    void execute() {
        while (!K) {
            Byte opcode = fetchByte();
            cycles++;
            if (instructions.count(opcode))
                instructions[opcode]();
        }
    }

    // jump to subroutine
    void jumpSubRoutine();

    // load A
    void loadAImmediate();
    void loadAZeroPage();
    void loadAZeroPageX();
    void loadAAbsolute();
    void loadAAbsoluteX();
    void loadAAbsoluteY();
    void loadAIndexedIndirect();
    void loadAIndirectIndexed();

    // load X
    void loadXImmediate();
    void loadXZeroPage();
    void loadXZeroPageY();
    void loadXAbsolute();
    void loadXAbsoluteY();

    // load Y
    void loadYImmediate();
    void loadYZeroPage();
    void loadYZeroPageX();
    void loadYAbsolute();
    void loadYAbsoluteX();

    // store X
    void storeXZeroPage();
    void storeXZeroPageY();
    void storeXAbsolute();

    // return from subroutine
    void returnFromSubRoutine();

    void kill();

    Byte getA() { return A; }
    Byte getX() { return X; }
    Byte getY() { return Y; }
    unsigned long getCycles() { return cycles; }
};

#endif
