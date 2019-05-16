#ifndef BoardClass_h
#define BoardClass_h


#include "Arduino.h"
#include <Wire.h>

typedef boolean bool;

class Simulator;

struct Executable {
  void execute(void);
};

struct RegistryInterface {
  bool register(Executable *);
};

struct NullRegistry : public RegistryInterface
{
  bool register(Executable *) {}
} nullRegistry;

struct Message {
  int a, b, c, d, e, f;
};

struct FailureHandler {
  void detect(int, Message);
};

class Protocol : public Executable {
private:
  int buffer[17];
public:
  bool isMessageValid(void);

  Protocol(RegistryInterface * ri)
  {
    ri->register(this);
  }

  Message receive(int id)
  {
    // Message <- buffer
    // clear buffer/message
  }

  void execute(void)
  {
    // receive single bytes over wire
  }
};

class Board : public Executable {
  int id;
  FailureHandler handler;
  Message message;
  Protocol protocol;

  Board() : id(0), protocol(&nullRegistry) {}

  Board(int id, RegistryInterface * ri)
    : protocol(ri)
  {
    this->id = id;
  }

  void execute(void)
  {
    // assert(id != 0);

    if (protocol.isMessageValid())
    {
      message = protocol.receive(id);
      handler.detect(id, message);
    }
  }
};

class Simulator : public RegistryInterface
{
private:
  Board boards[];
  std::list<Executable*> schedule;

  bool register(Executable * e)
  {
    schedule.push_back(e);
  }

  Simulator()
    : boards { Board(1, this) }
  {
    register(boards[0]);

    while(forever)
    {
      for(auto s : schedule)
      {
        s.execute();
      }
    }

  }


}

Simulator s;

Board b;
Board b1(1, &s);




class Board {
private:
int _received_Data[4];
int _Address;
int ID;
Simulator *m_sim;

public:

Board(int Address, int received_Data[4]);

void create_ID();


};


#endif
