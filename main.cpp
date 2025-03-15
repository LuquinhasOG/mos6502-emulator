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
    mem[0x0200] = LDY_I;
    mem[0x0201] = 0x1F;
    mem[0x0202] = LDX_ZPY;
    mem[0x0203] = 0x2F;
    mem[0x0204] = RTS;
    mem[0x004E] = 0xA3;

    cout << "installed program" << endl;
    cpu.execute(25);
    cout << "end of program" << endl << endl;

    cout << "registers: " << endl
        << "A = " << (int) cpu.getA() << endl
        << "X = " << (int) cpu.getX() << endl
        << "Y = " << (int) cpu.getY() << endl;

    return 0;
}
