
//#include "Arduino.h"
//#include <Wire.h>
#include <iostream>
#include <list>

using namespace std;


//typedef boolean bool;

class Simulator;

// interfaces begin
struct Executable {
        virtual void execute(void) = 0; // abstract method as interface
        virtual ~Executable() {
        }
};
// interfaces end

struct Message {
        int a, b, c, d, e, f;
};

struct FailureHandler {
        virtual void detect(int, Message) {
        }
};

class Protocol : public Executable {
private:
int buffer[17];
public:
bool isMessageValid(void) {
        return true;
}

Protocol()
{
}

Message receive(int id)
{
        // Message <- buffer
        // clear buffer/message
        std::cout << "Receive protocol\n";

}

void execute(void)
{
        // receive single bytes over wire
        std::cout << "Hello protocol\n";
}
};

class Board : public Executable {
int id;
FailureHandler handler;
Message message;
Protocol protocol;

public:
Board() : id(0), protocol() {
}

Board(int id)
        : protocol()
{
        this->id = id;
}

void execute(void)
{
        // assert(id != 0);
        protocol.execute();

        if (protocol.isMessageValid())
        {
                message = protocol.receive(id);
                handler.detect(id, message);
        }
}
};

class Simulator
{
private:
std::list<Board*> boards;
std::list<Executable*> schedule;

bool reg (Executable * e)
{
        schedule.push_back(e);
}

enum { forever = 1 };

public:
Simulator()
{
        // init
        Board board(1);
        boards.push_back(&board);
        reg (&board);

        std::cout << "Simulator running";
// sim
        do
        {
                for(auto s : schedule)
                {
                        s->execute();
                }
        }
        while(forever);

}


};

int main()
{
        Simulator s;

        Board b;
        Board b1(1);

        return 0;
}
