#pragma once

#include "Replace_Random.h"
#include "Replace_LRU.h"
#include "Replace_PLRU.h"

using namespace std;

// Global Variable
int tag_length;

class Cache
{
private:
    int cache_size; // 128 * 1K Byte
    int block_size; // 8B, 32B, 64B(3, 5, 8)
    int way_num;    // 1, 4, 8 ?
    int set_num;    // Cache set number: 2048
    ReplacePolicy replace_policy;
    bool write_through;
    bool write_allocate;
    Line **data;
    ReplaceHandler *repHandler;
    int offset_len; // Cache Offset length: 3
    int index_len;  // Cache Index Length: 11
    uint64 index_mask;

public:
    Cache(int cs, int bs, int ws, ReplacePolicy ra, bool wt, bool wa)
        : cache_size(cs), block_size(bs), way_num(ws), replace_policy(ra), write_through(wt), write_allocate(wa)
    {
        set_num = cache_size / (block_size * way_num);
        init();
    }

    int lookUpSet(uint32 index, uint64 tag)
    {
        Line *l = data[index];

        for (int i = 0; i < way_num; i++)
        {
            if (l[i].isValid() && (l[i].getTag() == tag))
            {
                return i;
            }
        }

        return -1;
    }

    int getVictimWayIndex(int set_index, uint64 addr, CmdType cmd)
    {
        Line *l = data[set_index];
        for (int i = 0; i < way_num; i++)
        {
            if (l[i].isValid() == false)
            {
                return i;
            }
        }
        return repHandler->getVictim(set_index, l, addr, cmd);
    }

    bool get(uint64 addr, CmdType cmd)
    {
        Line *pline = nullptr;


        bool hit = true;
        // cout << hex << "\naddr:" << addr << endl;

        uint32 set_index = getIndex(addr);
        // cout << hex <<"set_index:"<< set_index << endl;
        uint64 tag = getTag(addr);
        // cout << hex << "tag:"<<tag << endl;

        int way_index = lookUpSet(set_index, tag);
        // cout << "way_index:" << way_index << endl;
        if (way_index == -1)
        {
            hit = false;
            // 没找着，替换
            if (cmd == READ || (write_allocate))
            {
                way_index = getVictimWayIndex(set_index, addr, cmd);
                // cout << "way_index VICTIM:" << way_index << endl;
                if (way_index != -1)
                {
                    // 替换
                    pline = &data[set_index][way_index];
                    pline->setValid(true);
                    pline->setTag(tag);
                    // cout << "After Replace tag:" <<hex<< pline->getTag() << endl;
                    repHandler->update(set_index, way_index, pline, cmd, hit);
                }
            }
        }
        else
        {
            // 找着了
            hit = true;
            pline = &data[set_index][way_index];
            repHandler->update(set_index, way_index, pline, cmd, hit);
        }
        return hit;
    }

private:
    void init()
    {
        offset_len = msb(block_size);
        index_len = msb(set_num);
        tag_length = 64 - index_len - offset_len;
        index_mask = (1 << index_len) - 1;

        data = new Line *[set_num];

        for (int i = 0; i < set_num; i++)
        {
            data[i] = new Line[way_num];
        }
        switch (replace_policy)
        {
        case RANDOM:
            repHandler = new Replace_Random(set_num, way_num, replace_policy);
            break;
        case LRU:
            repHandler = new Replace_LRU(set_num, way_num, replace_policy);
            break;
        case PLRU:
            repHandler = new Replace_PLRU(set_num, way_num, replace_policy);
            break;
        default:
            printf("Wrong Replacement Policy");
            exit(-1);
            break;
        }
    }

    uint64 getTag(uint64 addr)
    {
        return ((addr >> offset_len) >> index_len);
    }

    uint32 getIndex(uint64 addr)
    {
        return ((addr >> offset_len) & index_mask);
    }
};
