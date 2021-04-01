#pragma once

#include "Replace_Random.h"
using namespace std;

// Global Variable


class Cache
{
private:
    //
    int cache_size; // 128 * 1K Byte
    int block_size; // 8B, 32B, 64B(3, 5, 8)
    int way_num;    // 0, 1, 4, 8 ?
    int set_num;    // Cache set number: 2048
    ReplacePolicy replace_policy;
    bool write_through;
    bool write_allocate;
    //
    Line **data;
    ReplaceHandler *repHandler;
    //
    uint64 timer;
    int offset_len; // Cache Offset length: 3
    int index_len;  // Cache Index Length: 11
    uint64 index_mask;
    int tag_length;

public:
    Cache(int cs, int bs, int ws, ReplacePolicy ra, bool wt, bool wa)
        : cache_size(cs), block_size(bs), way_num(ws), replace_policy(ra), write_through(wt), write_allocate(wa)
    {
        set_num = cache_size / (block_size * way_num);
        init();
    }

    int lookUpSet(uint32 index, uint64 tag){
        Line* l = data[index];

        for (int i = 0; i < way_num; i++){
            if(l[i].isValid() && (l[i].getTag() == tag)){
                return i;
            }
        }

        return -1;
    }

    int getVictimWayIndex(int set_index, uint64 addr, CmdType cmd){
        Line* l = data[set_index];
        for(int i = 0; i < way_num; i++){
            if(!l[i].isValid()){
                return i;
            }
        }
        return -1;
    }

    bool get(uint64 addr, CmdType cmd)
    {
        Line *pline = nullptr;
        timer++;
        repHandler->tick();

        bool hit = true;
        uint32 set_index = getIndex(addr);
        uint64 tag = getTag(addr);

        int way_index = lookUpSet(set_index, tag);
        if(way_index == -1){
            hit = false;
            // 没找着，替换
            if(write_allocate || (cmd == READ)){
                way_index = getVictimWayIndex(set_index, addr, cmd);
                if(way_index != -1){
                    // 替换
                    pline = &data[set_index][way_index];
                    pline->setValid(true);
                    pline->setTag(tag);
                    repHandler->update(set_index, way_index, pline, cmd, hit);
                }
            }
        }else{
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
        timer = 0;

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
