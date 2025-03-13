#include <iostream>
#include <functional>
#include <unordered_map>
#include "lib/memory.h"
#include "lib/cpu.h"

using namespace std;

int main() {
    Memory mem;
    CPU cpu(mem);

    cout << "launching" << endl;

    cpu.reset();
    mem[0xFFFC] = JSR;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x02;
    mem[0x0200] = LDX_I;
    mem[0x0201] = 0x0A;
    mem[0x0202] = LDA_INDX;
    mem[0x0203] = 0x10;
    mem[0x0204] = RTS;
    mem[0x001A] = 0x20;
    mem[0x001B] = 0x03;
    mem[0x0320] = 0x30;
    cout << "installed program" << endl;
    cpu.execute(20);
    cout << "end of program" << endl << endl;

    cout << "registers: " << endl << (int) cpu.getA() << endl << (int) cpu.getX() << endl << (int) cpu.getY() << endl;

    return 0;
}
