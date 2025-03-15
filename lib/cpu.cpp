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
    A = readByte(PAGE_ZERO | fetchByte());
    setLoadFlags(A);
    cycles += 2;
}

void CPU::loadAZeroPageX() {
    Byte zp_address = (PAGE_ZERO | fetchByte()) + X;
    A = readByte(zp_address);
    setLoadFlags(A);
    cycles += 3;
}

void CPU::loadAAbsolute() {
    A = readByte(fetchWord());
    setLoadFlags(A);
    cycles += 3;
}

void CPU::loadAAbsoluteX() {
    Word address = fetchWord();
    A = readByte(address + X);
    setLoadFlags(A);

    cycles += 3;
    if ((address & 0xFF00) != (address+X & 0xFF00))
        cycles++;
}

void CPU::loadAAbsoluteY() {
    Word address = fetchWord();
    A = readByte(address + Y);
    setLoadFlags(A);

    cycles += 3;
    if ((address & 0xFF00) != (address+Y & 0xFF00))
        cycles++;
}

void CPU::loadAIndexedIndirect() {
    Word address = readWord(0x00FF & (fetchByte() + X));
    A = readByte(address);
    setLoadFlags(A);
    cycles += 5;
}

void CPU::loadAIndirectIndexed() {
    Byte zp_address = fetchByte();
    Word low = readByte(zp_address) + Y;
    Byte carry = (0xFF00 & low) >> 8;
    Byte high = readByte(zp_address+1) + carry;
    Word effective_address = (0x00FF & low) | (high << 8);
    A = readByte(effective_address);
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
    X = readByte(PAGE_ZERO | fetchByte());
    setLoadFlags(X);
    cycles += 2;
}

void CPU::loadXZeroPageY() {
    Byte zp_address = (PAGE_ZERO | fetchByte()) + Y;
    X = readByte(zp_address);
    setLoadFlags(X);
    cycles += 3;
}

void CPU::loadXAbsolute() {
    X = readByte(fetchWord());
    setLoadFlags(X);
    cycles += 3;
}

void CPU::loadXAbsoluteY() {
    Word address = fetchWord();
    X = readByte(address + Y);
    setLoadFlags(X);

    cycles += 3;
    if ((address & 0xFF00) != (address+Y & 0xFF00))
        cycles++;
}

// load to Y register
void CPU::loadYImmediate() {
    Y = fetchByte();
    setLoadFlags(Y);
    cycles++;
}

void CPU::loadYZeroPage() {
    Y = readByte(PAGE_ZERO | fetchByte());
    setLoadFlags(Y);
    cycles += 2;
}

void CPU::loadYZeroPageX() {
    Byte zp_address = (PAGE_ZERO | fetchByte()) + X;
    Y = readByte(zp_address);
    setLoadFlags(Y);
    cycles += 3;
}

void CPU::loadYAbsolute() {
    Y = readByte(fetchWord());
    setLoadFlags(Y);
    cycles += 3;
}

void CPU::loadYAbsoluteX() {
    Word address = fetchWord();
    Y = readByte(address + X);
    setLoadFlags(Y);

    cycles += 3;
    if ((address & 0xFF00) != (address+X & 0xFF00))
        cycles++;
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
