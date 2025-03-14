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
    mem[0x0200] = LDA_INDY;
    mem[0x0201] = 0x00;
    mem[0x0202] = RTS;
    mem[0x0000] = 0x0A;
    mem[0x0001] = 0x10;
    mem[0x100A] = 0x28;
    cout << "installed program" << endl;
    cpu.execute(17);
    cout << "end of program" << endl << endl;

    cout << "registers: " << endl
        << "A = " << (int) cpu.getA() << endl
        << "X = " << (int) cpu.getX() << endl
        << "Y = " << (int) cpu.getY() << endl;

    return 0;
}
