#include "cache.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

int cache_size = 128 * 1024;
int block_size = 8;
int way_num = 8;
ReplacePolicy po = PLRU;
bool wt = false;
bool wa = true;

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

void run(string path, string outpath)
{
    bool Log = false;
    if (outpath != "None")
    {
        Log = true;
        cout << "Log" << endl;
    }
    ifstream fin;
    fin.open(path.c_str());
    uint64 addr, count = 0, hitc = 0, misc = 0;
    ofstream fout;
    if (Log)
        fout.open(outpath.c_str());
    // cache_size(cs), block_size(bs), way_num(ws),
    // replace_policy(ra), write_through(wt), write_allocate(wa)
    Cache cache(cache_size, block_size, way_num, po, wt, wa);
    string addrstr, cmd;
    CmdType cmdtype;
    while (fin >> addrstr >> cmd)
    {
        // cout << addrstr << ' '<<cmd <<endl;
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
            if (Log)
                fout << "Hit\n";
        }
        else
        {
            misc++;
            if (Log)
                fout << "Miss\n";
        }
        count++;
        if (count % 10000 == 0)
        {
            // printf("count = %llu, hitc = %llu, misc = %llu, miss rate = %.2f\n", count, hitc, misc, misc * 1.0 / count * 100);
        }
    }

    // printf("count = %llu, miss rate = %.4f\n", count, misc * 1.0 / count * 100);
    printf("%.2f%%\t", misc * 1.0 / count * 100);
    fin.close();
    if (Log)
        fout.close();
}

// block_size way_num wa policy path outpath
int main(int argc, char **argv)
{
    // for(int i = 0 ;i < argc;i++){
    //     cout << "args: " << argv[i] <<endl;
    // }
    if (argc != 7)
        exit(-1);
    cache_size = 128 * 1024;
    block_size = atoi(argv[1]);
    way_num = atoi(argv[2]);
    wa = (argv[3][0] == 'a');
    string strp(argv[4]);
    if (strp == "LRU")
    {
        po = LRU;
    }
    else if (strp == "PLRU")
    {
        po = PLRU;
    }
    else if (strp == "RANDOM")
    {
        po = RANDOM;
    }
    else
    {
        po = RANDOM;
    }
    string path(argv[5]);
    string outpath(argv[6]);
    // cout << path <<endl;
    // string path = "/Users/wmhst7/Desktop/SysCourse/exp1/SimpleCache/test_trace/1.trace";
    // string outpath = "/Users/wmhst7/Desktop/SysCourse/exp1/SimpleCache/out/1.log";
    // outpath = string("None");
    run(path, outpath);
    return 0;
}
