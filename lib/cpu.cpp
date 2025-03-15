#include <functional>
#include <unordered_map>
#include "memory.h"
#include "cpu.h"

constexpr Word
    PAGE_ZERO = 0x0000,
    PAGE_ONE = 0x0100,
    STACK_BOTTOM = 0x01FF;

// implementation instruction functions

// jump to subroutine
void CPU::jumpSubRoutine(int& cycles) {
    Word address = fetchWord(cycles);
    SP -= 2;
    writeWord(cycles, PC, (PAGE_ONE | SP + 1));
    PC = address;
    cycles--;
}

// load to A register
void CPU::loadAImmediate(int& cycles) {
    A = fetchByte(cycles);
    setLoadFlags(A);
}

void CPU::loadAZeroPage(int& cycles) {
    A = readByte(cycles, (PAGE_ZERO | fetchByte(cycles)));
    setLoadFlags(A);
}

void CPU::loadAZeroPageX(int& cycles) {
    Byte zp_address = (PAGE_ZERO | fetchByte(cycles)) + X;
    cycles--;
    A = readByte(cycles, zp_address);
    setLoadFlags(A);
}

void CPU::loadAAbsolute(int& cycles) {
    A = readByte(cycles, fetchWord(cycles));
    setLoadFlags(A);
}

void CPU::loadAAbsoluteX(int& cycles) {
    Word address = fetchWord(cycles);
    A = readByte(cycles, (address + X));
    setLoadFlags(A);

    if ((address & 0xFF00) != (address+X & 0xFF00))
        cycles--;
}

void CPU::loadAAbsoluteY(int& cycles) {
    Word address = fetchWord(cycles);
    A = readByte(cycles, (address + Y));
    setLoadFlags(A);

    if ((address & 0xFF00) != (address+Y & 0xFF00))
        cycles--;
}

void CPU::loadAIndexedIndirect(int& cycles) {
    Word address = readWord(cycles, (0x00FF & (fetchByte(cycles) + X)));
    A = readByte(cycles, address);
    cycles--;
    setLoadFlags(A);
}

void CPU::loadAIndirectIndexed(int& cycles) {
    Byte zp_address = fetchByte(cycles);
    Word low = readByte(cycles, zp_address) + Y;
    Byte carry = (0xFF00 & low) >> 8;
    Byte high = readByte(cycles, zp_address+1) + carry;
    Word effective_address = (0x00FF & low) | (high << 8);
    A = readByte(cycles, effective_address);
    setLoadFlags(A);

    if (carry)
        cycles--;
}

// load to X register
void CPU::loadXImmediate(int& cycles) {
    X = fetchByte(cycles);
    setLoadFlags(X);
}

void CPU::loadXZeroPage(int& cycles) {
    X = readByte(cycles, (PAGE_ZERO | fetchByte(cycles)));
    setLoadFlags(X);
}

void CPU::loadXZeroPageY(int& cycles) {
    Byte zp_address = (PAGE_ZERO | fetchByte(cycles)) + Y;
    cycles--;
    X = readByte(cycles, zp_address);
    setLoadFlags(X);
}

void CPU::loadXAbsolute(int& cycles) {
    X = readByte(cycles, fetchWord(cycles));
    setLoadFlags(X);
}

void CPU::loadXAbsoluteY(int& cycles) {
    Word address = fetchWord(cycles);
    X = readByte(cycles, (address + Y));
    setLoadFlags(X);

    if ((address & 0xFF00) != (address+Y & 0xFF00))
        cycles--;
}

// load to Y register
void CPU::loadYImmediate(int& cycles) {
    Y = fetchByte(cycles);
    setLoadFlags(Y);
}

void CPU::loadYZeroPage(int& cycles) {
    Y = readByte(cycles, (PAGE_ZERO | fetchByte(cycles)));
    setLoadFlags(Y);
}

void CPU::loadYZeroPageX(int& cycles) {
    Byte zp_address = (PAGE_ZERO | fetchByte(cycles)) + X;
    cycles--;
    Y = readByte(cycles, zp_address);
    setLoadFlags(Y);
}

void CPU::loadYAbsolute(int& cycles) {
    Y = readByte(cycles, fetchWord(cycles));
    setLoadFlags(Y);
}

void CPU::loadYAbsoluteX(int& cycles) {
    Word address = fetchWord(cycles);
    Y = readByte(cycles, (address + X));
    setLoadFlags(Y);

    if ((address & 0xFF00) != (address+X & 0xFF00))
        cycles--;
}

// return from subroutine
void CPU::returnFromSubRoutine(int& cycles) {
    Word rtn_address = readWord(cycles, (PAGE_ONE | SP + 1));
    SP++;
    mem[PAGE_ONE | SP] = 0x00;
    SP++;
    mem[PAGE_ONE | SP] = 0x00;
    PC = rtn_address;
    cycles -= 3;
};
