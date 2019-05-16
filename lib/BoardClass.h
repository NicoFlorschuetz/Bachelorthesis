#ifndef BoardClass_h
#define BoardClass_h


#include "Arduino.h"
#include <Wire.h>

typedef boolean bool;

class Simulator;

struct Message {
  int a, b, c, d, e, f;
};

struct FailureHandler {
  void detect(int, Message);
};

class Protocol {
private:
  int buffer[17];
public:
  bool isMessageValid(void);

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

class Board {
  int id;
  FailureHandler handler;
  Message message;
  Protocol protocol;

  Board() : id(0) {}

  Board(int id)
  {
    this->id = id;
  }

  void execute()
  {
    // assert(id != 0);

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
  Board boards[];

  Simulator()
  {
    board[0].execute();

  }


}




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
