#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>

#define uint8 uint8_t
#define uint32 uint32_t
#define uint64 uint64_t

enum ReplacePolicy
{
    PLRU,
    LRU,
    RANDOM,
};

enum CmdType
{
    READ,
    WRITE,
};

inline int msb(uint32 n) {
    int p = 0;

    if (n == 0)
        return -1;

    if (n & 0xffff0000) {
        p += 16;
        n >>= 16;
    }
    if (n & 0x0000ff00) {
        p += 8;
        n >>= 8;
    }
    if (n & 0x000000f0) {
        p += 4;
        n >>= 4;
    }
    if (n & 0x0000000c) {
        p += 2;
        n >>= 2;
    }
    if (n & 0x00000002) {
        p += 1;
    }

    return p;
}

