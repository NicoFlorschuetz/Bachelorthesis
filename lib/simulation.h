#ifndef simulation_h
#define simulation_h

#include "Arduino.h"
#include <Wire.h>


//message for I2C to slave
typedef enum MessageToSlave {
        NO_MSG = 0,
        RETURN_MSG = 1,
}messageToSlave;

//different reset modes of slaves
typedef enum eFdirAction {
        STANDARD_FDIR = 0,
        WIRE_FDIR = 1
}FDIR_ACTION_t;

struct Executable {
        virtual void execute(void) = 0; // abstract method as interface
        virtual ~Executable() {
        }
};

struct FdirAction : public Executable {
        virtual void reset(void) = 0;
        virtual ~FdirAction(){
        }
};

//struct which defines the structure of message
struct Message {
        int id_board, failureCode, failureSolved, value;
};

//struct which defines the structure of first received setup_Message
struct Setup_Message {
        int keepAlivePin, fdirActionParam;
        FDIR_ACTION_t fdirActions;
};

//defines FailureLevel
struct FailureAnalysis {
        bool LevelNull = false;
        bool LevelOne = false;
        bool LevelTwo = false;
        bool LevelThree = false;
        bool LevelFour = false;
};

struct healthStatus {
        bool health = true;
};

struct FailureHandler {
        virtual FailureAnalysis detect(int, Message);
        bool LevelSent;
};

class FDIR_Master;
class Protocol {
private:
int buffer[7];
int id;

public:
bool isMessageValid(void);
Protocol();
Setup_Message receiveSetup();
Message receive();
void readData(int id);
void sendMessage(int,  MessageToSlave);
};

class Board : public Executable {
int id;
FailureHandler handler;
Message message;
Setup_Message setup_Message;
Protocol protocol;
FailureAnalysis failureLevel;

public:
healthStatus healthstatus;

FdirAction* fdirActions;
Board() : id(0){
}
Board(int id) : protocol(){
        this->id = id;
        this->fdirActions = NULL;

}

void execute(void);

};

class FdirActionFactory {
public:
static FdirAction* getFdirAction(FDIR_ACTION_t, int);

};

class PinReset : public FdirAction {
private:
int pin;
public:
PinReset(int pin){
        this->pin = pin;
}
void execute(void);
void reset(void);
virtual ~PinReset() {
}
};

class WireReset : public FdirAction {
private:
int id;
Protocol protocol;
bool messageSent;
public:
WireReset(int id) : protocol(){
        this->id = id;
        this->messageSent = false;
}

void execute(void);
void reset(void);

};

class FDIR_Master {
private:
Board *boards[10];
Executable *schedule[10];
bool reg(Executable* e);
volatile int counter_first = 0;
volatile int counter_second = 0;
int anzahlBoards = 0;

public:
FDIR_Master();
void setup_pins();
void searchForAddresses();
void doScheduling();
};

#endif
