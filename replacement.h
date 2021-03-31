#include "defs.h"
#include "Line.h"

class ReplaceHandler{
protected:
    uint64 timer;
public:
    int way_num;


    ReplaceHandler(){

    }

    virtual int getVictim() = 0;

    virtual void update() = 0;

    void setPolicy(ReplacePolicy p){}

    void tick(){timer++;}

    virtual void initial() = 0;
    
};