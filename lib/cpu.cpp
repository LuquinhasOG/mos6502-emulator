#include <iostream>
#include <functional>
#include <unordered_map>
#include "memory.h"
#include "cpu.h"

// implementation instruction functions

// jump to subroutine
void CPU::jumpSubRoutine() {
    Word address = fetchWord();
    SP -= 2;
    writeWord(PC, (PAGE_ONE | SP + 1));
    PC = address;
    cycles += 5;
}

// load to A register
void CPU::loadAImmediate() {
    A = fetchByte();
    setLoadFlags(A);
    cycles++;
}

void CPU::loadAZeroPage() {
    A = readByteZeroPage();
    setLoadFlags(A);
    cycles += 2;
}

void CPU::loadAZeroPageX() {
    A = readByteZeroPage(X);
    setLoadFlags(A);
    cycles += 3;
}

void CPU::loadAAbsolute() {
    A = readByteAbsolute();
    setLoadFlags(A);
    cycles += 3;
}

void CPU::loadAAbsoluteX() {
    A = readByteAbsolute(X);
    setLoadFlags(A);
    cycles += 3;
}

void CPU::loadAAbsoluteY() {
    A = readByteAbsolute(Y);
    setLoadFlags(A);
    cycles += 3;
}

void CPU::loadAIndexedIndirect() {
    A = mem[readWord(0x00FF & (fetchByte() + X))];
    setLoadFlags(A);
    cycles += 5;
}

void CPU::loadAIndirectIndexed() {
    Byte zp_address = fetchByte();
    Word low = mem[zp_address] + Y;
    Byte carry = (0xFF00 & low) >> 8;
    Word effective_address = (0x00FF & low) | ((mem[zp_address+1] + carry) << 8);
    A = mem[effective_address];
    setLoadFlags(A);

    cycles += 4;
    if (carry)
        cycles++;
}

// load to X register
void CPU::loadXImmediate() {
    X = fetchByte();
    setLoadFlags(X);
    cycles++;
}

void CPU::loadXZeroPage() {
    X = readByteZeroPage();
    setLoadFlags(X);
    cycles += 2;
}

void CPU::loadXZeroPageY() {
    X = readByteZeroPage(Y);
    setLoadFlags(X);
    cycles += 3;
}

void CPU::loadXAbsolute() {
    X = readByteAbsolute();
    setLoadFlags(X);
    cycles += 3;
}

void CPU::loadXAbsoluteY() {
    X = readByteAbsolute(Y);
    setLoadFlags(X);
    cycles += 3;
}

// load to Y register
void CPU::loadYImmediate() {
    Y = fetchByte();
    setLoadFlags(Y);
    cycles++;
}

void CPU::loadYZeroPage() {
    Y = readByteZeroPage();
    setLoadFlags(Y);
    cycles += 2;
}

void CPU::loadYZeroPageX() {
    Y = readByteZeroPage(X);
    setLoadFlags(Y);
    cycles += 3;
}

void CPU::loadYAbsolute() {
    Y = readByteAbsolute();
    setLoadFlags(Y);
    cycles += 3;
}

void CPU::loadYAbsoluteX() {
    Y = readByteAbsolute(X);
    setLoadFlags(Y);
    cycles += 3;
}

// return from subroutine
void CPU::returnFromSubRoutine() {
    Word rtn_address = readWord(PAGE_ONE | SP + 1);
    SP++;
    mem[PAGE_ONE | SP] = 0x00;
    SP++;
    mem[PAGE_ONE | SP] = 0x00;
    PC = rtn_address;
    cycles += 5;
};

void CPU::kill() {
    K = 1;
}
