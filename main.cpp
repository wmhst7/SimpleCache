#include "cache.h"
// #include "defs.h"
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

uint64 str2ull(string str)
{
    uint64 res = 0;
    for (int i = 2; i < str.size(); i++)
    {
        res *= 2;
        res += (str[i] - '0');
    }
    return res;
}

void run(string path)
{
    freopen(path.c_str(), "r", stdin);
    uint64 addr, count = 0, hitc = 0, misc = 0;

    // cache_size(cs), block_size(bs), way_num(ws), 
    // replace_policy(ra), write_through(wt), write_allocate(wa)
    int cache_size = 128 * 1024;
    int block_size = 8;
    int way_num = 8;
    ReplacePolicy po = RANDOM;
    bool wt = false;
    bool wa = false;

    Cache cache(cache_size, block_size, way_num, po, wt, wa);
    string addrstr, cmd;
    CmdType cmdtype;
    while (cin >> addrstr >> cmd)
    {
        addr = str2ull(addrstr);
        if (cmd == "w")
        {
            cmdtype = WRITE;
        }
        else if (cmd == "r")
        {
            cmdtype = READ;
        }
        else
        {
            cout << "cmd error" << endl;
            exit(-1);
        }
        if (cache.get(addr, cmdtype))
        {
            hitc++;
        }
        else
        {
            misc++;
        }
        count++;
        if (count % 1000 == 0)
        {
            printf("count = %llu, miss rate = %f\n", count, misc * 1.0 / count * 100);
        }
    }

    printf("count = %llu, miss rate = %f\n", count, misc * 1.0 / count * 100);
}

int main(int argc, char **argv)
{

    string path = "./test_trace/";
    vector<string> files = {"1.trace", "2.trace", "3.trace", "4.trace"};
    for (auto file : files)
    {
        run(path + file);
    }

    return 0;
}