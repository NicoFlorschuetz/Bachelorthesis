#include "Wire.h"

#include "/Users/nicoflorschutz/Documents/Bachelorthesis/eigeneBib/classMega.h"


#ifndef BOARDCLASS_H
#define BOARDCLASS_H

class Board : public Simulator {
private:
int add;
boolean reset, power;
public:
Board(int address, boolean resetStatus, boolean powerStatus) : Simulator(a,b,res,pow){
        add = address;
        reset = resetStatus;
        power = powerStatus;
};


};

#endif
