#ifndef CPU_H
#define CPU_H

#define inst_ref(func) std::bind(&func, this, std::placeholders::_1)

constexpr Byte
    JSR = 0x20,
    LDA_I = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    LDA_A = 0x6D,
    RTS = 0x60;

class CPU {
private:
    // registers
    Word PC;
    Byte SP, A, X, Y;

    // processor status register
    bool C, Z, I, D, B, O, N;

    std::unordered_map<Byte, std::function<void(u32&)>> instructions = {
        {JSR, inst_ref(jumpSubRoutine)},
        {LDA_I, inst_ref(loadAImmediate)},
        {LDA_ZP, inst_ref(loadAZeroPage)},
        {LDA_ZPX, inst_ref(loadAZeroPageX)},
        {LDA_A, inst_ref(loadAAbsolute)},
        {RTS, inst_ref(returnFromSubRoutine)},
    };

    Memory& mem;

public:
    CPU(Memory& memory): mem(memory) {}

    void reset() {
        PC = 0xFFFC;
        SP = 0x00;
        A = X = Y = 0;
        clearFlags();
        mem.reset();
    }

    Byte fetchByte(u32& cycles) {
        Byte b = mem[PC];
        PC++;
        cycles--;

        return b;
    }

    Word fetchWord(u32& cycles) {
        Word w = mem[PC];
        PC++;
        w |= (mem[PC] << 8);
        PC++;
        cycles -= 2;

        return w;
    }

    Byte readByte(u32& cycles, Word address) {
        Byte b = mem[address];
        cycles--;
        return b;
    }

    Byte readWord(u32& cycles, Word address) {
        Word w = mem[address] | (mem[address + 1] << 8);
        cycles -= 2;

        return w;
    }

    void writeWord(u32& cycles, Word w, Word address) {
        mem[address] = w & 0x00FF; // mask convert word to byte
        mem[address + 1] = (w >> 8);
        cycles -= 2;
    }

    void clearFlags() {
        C = Z = I = D = B = O = N = 0;
    }

    void setLoadFlags() {
        Z = (A == 0);
        N = ((A & 0x80) == 0x80);
    }

    void execute(u32 cycles) {
        while (cycles > 0) {
            Byte opcode = fetchByte(cycles);
            if (instructions.count(opcode))
                instructions[opcode](cycles);
        }
    }

    void jumpSubRoutine(u32& cycles);
    void loadAImmediate(u32& cycles);
    void loadAZeroPage(u32& cycles);
    void loadAZeroPageX(u32& cycles);
    void loadAAbsolute(u32& cycles);
    void returnFromSubRoutine(u32& cycles);

    Byte getA() {
        return A;
    }
};

#endif
