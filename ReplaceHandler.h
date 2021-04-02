#pragma once
#include "defs.h"
#include "Line.h"

int _way_num;

class ReplaceHandler
{
protected:
    int set_num;
    int way_num;
    ReplacePolicy policy;

public:
    ReplaceHandler(int a, int b, ReplacePolicy c) : set_num(a), way_num(b), policy(c)
    {
    }

    void setPolicy(ReplacePolicy p)
    {
        policy = p;
    }

    void init(){};

    virtual int getVictim(int set_index, const Line *linep, uint64 addr, CmdType cmd) = 0;

    virtual void update(int set_index, int way_index, const Line *linep, CmdType cmd, bool hit) = 0;
};