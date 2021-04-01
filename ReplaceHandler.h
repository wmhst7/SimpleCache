#include "defs.h"
#include "Line.h"

class ReplaceHandler
{
protected:
    uint64 timer;
    int set_num;
    int way_num;
    ReplacePolicy policy;

public:
    ReplaceHandler(int a, int b, ReplacePolicy c) : set_num(a), way_num(b), policy(c)
    {
        timer = 0;
    }

    virtual int getVictim(int set_index, const Line *linep, uint64 addr, CmdType cmd) = 0;

    virtual void update(int set_index, int way_index, const Line *linep, CmdType cmd, bool ishit) = 0;

    void setPolicy(ReplacePolicy p)
    {
        policy = p;
    }

    void tick() { timer++; }

    virtual void init() = 0;
};