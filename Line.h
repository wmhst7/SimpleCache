#pragma once
#include "defs.h"
#include <iostream>
#include <string>

extern int tag_length;

class Line
{
public:
    unsigned char data[7];

    Line()
    {
    }

    bool isValid()
    {
        return data[0] & 1U;
    }

    void setValid(bool v)
    {
        if (v)
            data[0] |= 1;
        else
            data[0] &= (~(uint8)(1));
    }
    bool isDirty()
    {
        return (data[6] >> 7) & (1U);
    }
    void setDirty(bool v)
    {
        if (v)
            data[6] |= (1 << 7);
        else
            data[6] &= (~((uint8)1 << 7));
    }

    // The first tag_length bits are available
    uint64 getTag()
    {
        uint64 mask = ((uint64)(1) << (tag_length)) - 1;
        uint64 tag = ((*(uint64 *)(data)) >> 1);
        return tag & mask;
    }

    void setTag(uint64 tag)
    {
        bool v = isValid();
        bool d = isDirty();
        (*(uint64 *)(data)) = (tag << 1);
        setValid(v);
        setDirty(d);
    }
};