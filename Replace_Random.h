#pragma once
#include "ReplaceHandler.h"
using namespace std;

class Replace_Random : public ReplaceHandler
{
public:
    Replace_Random(int s, int w, ReplacePolicy p) : ReplaceHandler(s, w, p)
    {
    }

    int getVictim(int set_index, const Line *linep, uint64 addr, CmdType cmd)
    {
        return (rand() % way_num);
    }

    void update(int set_index, int way_index, const Line *linep, CmdType cmd, bool hit)
    {
    }
};