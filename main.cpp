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
    mem[0xFFFF] = JAM;
    mem[0x0200] = 0x00;
    mem[0x0201] = 0x00;
    mem[0x0202] = LDY_I;
    mem[0x0203] = 0xF8;
    mem[0x0204] = STY_ZP;
    mem[0x0205] = 0x30;
    mem[0x0206] = LDA_ZP;
    mem[0x0207] = 0x30;
    mem[0x0208] = RTS;

    cout << "installed program" << endl;
    cpu.execute();
    cout << "end of program" << endl << endl;

    cout << (int) mem[0x0028] << endl;
    cout << "cycles performed: " << cpu.getCycles() << endl;
    cout << "registers: " << endl
        << "A = " << (int) cpu.getA() << endl
        << "X = " << (int) cpu.getX() << endl
        << "Y = " << (int) cpu.getY() << endl;

    return 0;
}
