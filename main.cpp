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
