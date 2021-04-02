#include <iostream>

#include "defs.h"
#include "Replace_Random.h"

using namespace std;

int main()
{
    Line l;
    uint64 addr = 0x7fff778852d8;
    cout << hex << addr <<endl;
    // cout << hex<< l.getTag() << " \n";
    l.setTag(addr);
    cout << hex << l.getTag() << " \n";
    l.setTag(addr);
    cout << l.isValid() << " \n";
    l.setValid(false);
    cout << hex << l.getTag() << " \n";
    cout << l.isValid() << " \n";
    l.setValid(true);
    cout << hex << l.getTag() << " \n";
    cout << l.isValid() << " \n";
    l.setTag(addr);
    cout << l.isValid() << " \n";
    cout << hex << l.getTag() << " \n";

    // printf("%x\n", (~((uint8)1 << 7)));
    printf("%d\n", msb(1));
    return 0;
}