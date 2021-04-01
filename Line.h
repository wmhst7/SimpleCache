#pragma once
#include "defs.h"
#include <iostream>
#include <string>

// extern int tag_length;

class Line
{
public:
    unsigned char data[7];

    Line()
    {
    }

    bool isValid()
    {
        return data[6] & 1U;
    }

    void setValid(bool v)
    {
        if (v)
            data[6] |= 1;
        else
            data[6] &= (~(uint8)(1));
    }
    bool isDirty()
    {
        return (data[0] >> 7) & 1U;
    }
    void setDirty(bool v)
    {
        if (v)
            data[0] |= (1 << 7);
        else
            data[0] &= (~((uint8)1 << 7));
    }

    // The first tag_length bits are available
    uint64 getTag()
    {
        return ((*(uint64 *)(data)) << 1) >> (8 + 1);
    }

    void setTag(uint64 addr)
    {
        bool v = isValid();
        bool d = isDirty();
        (*(uint64 *)(data)) = (addr << 9);
        setValid(v);
        setDirty(d);
    }
};