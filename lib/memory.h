#ifndef MEMORY_H
#define MEMORY_H

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

constexpr int MEM_SIZE = 65536;

class Memory {
private:
    Byte data[MEM_SIZE];

public:
    void reset() {
        for (u32 i = 0; i < MEM_SIZE; i++) {
            data[i] = 0;
        }
    }

    Byte operator[](Word address) const {
        return data[address];
    }

    Byte& operator[](Word address) {
        return data[address];
    }
};

#endif
