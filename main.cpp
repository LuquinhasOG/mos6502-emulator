#include <iostream>
#include <functional>
#include <unordered_map>
#include "lib/memory.h"

#define inst_ref(func) std::bind(&func, this, std::placeholders::_1)

using namespace std;

constexpr Word
    PAGE_ZERO = 0x0000,
    PAGE_ONE = 0x0100;
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

    std::unordered_map<Byte, function<void(u32&)>> instructions = {
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

// implementation instruction functions
void CPU::jumpSubRoutine(u32& cycles) {
    Word address = fetchWord(cycles);
    writeWord(cycles, PC, (PAGE_ONE | SP));
    SP += 2;
    PC = address;
    cycles--;
    clearFlags();
}

void CPU::loadAImmediate(u32& cycles) {
    A = fetchByte(cycles);
    setLoadFlags();
}

void CPU::loadAZeroPage(u32& cycles) {
    Byte zp_address = PAGE_ZERO | fetchByte(cycles);
    A = readByte(cycles, zp_address);
    setLoadFlags();
}

void CPU::loadAZeroPageX(u32& cycles) {
    Byte zp_address = PAGE_ZERO | fetchByte(cycles);
    zp_address += X;
    cycles--;
    A = readByte(cycles, zp_address);
    setLoadFlags();
}

void CPU::loadAAbsolute(u32& cycles) {
    A = readByte(cycles, fetchWord(cycles));
    setLoadFlags();
}

void CPU::returnFromSubRoutine(u32& cycles) {
    Word rtn_address = readWord(cycles, (PAGE_ONE | SP - 2));
    SP--;
    mem[PAGE_ONE | SP] = 0x00;
    SP--;
    mem[PAGE_ONE | SP] = 0x00;
    PC = rtn_address;
    clearFlags();
    cycles -= 3;
};

int main() {
    Memory mem;
    CPU cpu(mem);

    cout << "launching" << endl;

    cpu.reset();
    mem[0xFFFC] = JSR;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x02;
    mem[0x0200] = LDA_A;
    mem[0x0201] = 0x10;
    mem[0x0202] = 0x10;
    mem[0x0203] = RTS;
    mem[0x1010] = 0x0A;
    cout << "installed program" << endl;
    cpu.execute(16);
    cout << "end of program" << endl;

    cout << (int) cpu.getA() << endl;

    return 0;
}
