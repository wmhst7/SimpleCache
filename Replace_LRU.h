#pragma once
#include "ReplaceHandler.h"
using namespace std;

extern int _way_num;

class LRU_Queue
{
private:
    uchar *lru = nullptr;
    int way_len;

    int get_bit(int i)
    {
        return (lru[i / 8] >> (i % 8)) & 1U;
    }

    void set_bit(int i, bool v)
    {
        if (v)
        {
            lru[i / 8] |= (((uchar)(1)) << (i % 8));
        }
        else
        {
            lru[i / 8] &= (~(((uchar)(1)) << (i % 8)));
        }
    }

public:
    LRU_Queue()
    {
        way_len = msb(_way_num);
        way_len = (way_len == 0) ? 1 : way_len;
        int len = (_way_num * way_len + 7) / 8;
        len = (len == 0) ? 1 : len;
        lru = new uchar[len];
    }

    ~LRU_Queue()
    {
        delete[] lru;
    }

    int get(int pos)
    {
        uint64 ret = 0;
        for (int i = (pos + 1) * way_len - 1; i >= pos * way_len; i--)
        {
            ret <<= 1;
            ret |= get_bit(i);
        }
        return ret;
    }

    void set(int pos, int v)
    {
        for (int i = pos * way_len; i < (pos + 1) * way_len; i++)
        {
            set_bit(i, (v & 1) == 1);
            v >>= 1;
        }
    }
};

class Replace_LRU : public ReplaceHandler
{
public:
    LRU_Queue *q;

    Replace_LRU(int s, int w, ReplacePolicy p) : ReplaceHandler(s, w, p)
    {
        _way_num = way_num;
        q = new LRU_Queue[set_num];
        for (int i = 0; i < set_num; i++)
        {
            for (int j = 0; j < way_num; j++)
            {
                q[i].set(j, j);
            }
        }
    }

    int getVictim(int set_index, const Line *linep, uint64 addr, CmdType cmd)
    {
        for (int i = 0; i < way_num; i++)
        {
            if (q[set_index].get(i) == (way_num - 1))
            {
                return i;
            }
        }
        return 0;
    }

    void update(int set_index, int way_index, const Line *linep, CmdType cmd, bool hit)
    {
        int pos = q[set_index].get(way_index);
        for (int i = 0; i < way_num; i++)
        {
            int res = q[set_index].get(i);
            if (res < pos)
            {
                q[set_index].set(i, ++res);
            }
        }
        q[set_index].set(way_index, 0);
    }
};