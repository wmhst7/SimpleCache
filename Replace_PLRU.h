#pragma once
#include "ReplaceHandler.h"
using namespace std;

extern int _way_num;

class BinaryTree
{
private:
    uchar *tree;
    int depth;

    // 第i位的bit值
    bool get(int i)
    {
        int res = (tree[i / 8] >> (i % 8)) & 1U;
        return res == 1;
    }

    bool set(int i, bool v)
    {
        if (v)
        {
            tree[i / 8] |= (((uchar)1) << (i % 8));
        }
        else
        {
            tree[i / 8] &= (~(((uchar)1) << (i % 8)));
        }
        return true;
    }

public:
    BinaryTree()
    {
        depth = msb(_way_num);
        int len = _way_num / 8;
        len = (len == 0) ? 1 : len;
        tree = new uchar[len];
        for (int i = 0; i < len; i++)
        {
            tree[i] = 0;
        }
        memset(tree, 0, len * sizeof(uchar));
    }

    ~BinaryTree()
    {
        delete[] tree;
    }

    void update(int way_index)
    {
        int l[depth];
        int pos = 0;
        for(int i=0; i < depth; i++){
            l[i] = (way_index & 1U);
            way_index >>= 1;
        }
        for(int i = depth - 1; i >= 0; i--){
            set(pos, l[i] == 0);
            if(l[i] == 0) pos = 2 * pos + 1;
            else pos = 2 * pos + 2;
        }
    }

    uint32 getVictim(){
        uint32 res = 0;
        int pos = 0;
        for(int i = 0; i < depth; i++){
            res <<= 1;
            if(get(pos)){
                pos = 2 * pos + 1;
            }else{
                pos = 2 * pos + 2;
                res = res | 1U;
            }
        }
        return res;
    }
};

class Replace_PLRU : public ReplaceHandler
{
private:
    BinaryTree *trees;

public:
    Replace_PLRU(int s, int w, ReplacePolicy p) : ReplaceHandler(s, w, p)
    {
        _way_num = way_num;
        trees = new BinaryTree[set_num];
    }

    int getVictim(int set_index, const Line *linep, uint64 addr, CmdType cmd)
    {
        return trees[set_index].getVictim();
    }

    void update(int set_index, int way_index, const Line *linep, CmdType cmd, bool hit)
    {
        trees[set_index].update(way_index);
    }
};