#include "memory.h"
#include "cpu.h"

constexpr Word
    PAGE_ZERO = 0x0000,
    PAGE_ONE = 0x0100;

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
