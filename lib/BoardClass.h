#ifndef BoardClass_h
#define BoardClass_h

#include "simulation.h"


class Board : public Executable {
int id;
FailureHandler handler;
Message message;
Protocol protocol;
public:
Board() : id(0){
}

Board(int id) : protocol(){
        this->id = id;
}

void execute(void);


};


#endif
