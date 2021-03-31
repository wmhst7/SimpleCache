#pragma once
#include "defs.h"
#include "Line.h"
using namespace std;

// Global Variable
int tag_length;

class Cache
{
private:
    int cache_size; // 128KB
    int block_size; // 8B, 32B, 64B(3, 5, 8)
    int way_num;    // 0, 1, 4, 8 ?
    ReplacePolicy replace_policy;
    bool write_through;
    bool write_allocate;

    Line **data;

public:
    Cache(int bs, int ws, ReplacePolicy ra, bool wt, bool wa)
        : block_size(bs), way_num(ws), replace_policy(ra), write_through(wt), write_allocate(wa)
    {
    }
};
