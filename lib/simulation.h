#ifndef simulation_h
#define simulation_h

#include "Arduino.h"
#include <Wire.h>
//#include "BoardClass.h"



typedef enum pins_arduino {

        RESET_ARDUINO1 = 9,
        RESET_ARDUINO2 = 8,

}pins_arduino_t;

typedef enum InterruptPin {
        PIN_FIRST = 2,
        PIN_SECOND = 3,
}interruptPin;

struct Executable {
        virtual void execute(void) = 0; // abstract method as interface
        virtual ~Executable() {
        }
};

struct Message {
        int id_board, failureCode, failure, value;
};

struct FailureHandler {
        virtual void detect(int, Message);
};

struct FailureAnalysis {
        bool LevelNull = false;
        bool LevelOne = false;
        bool LevelTwo = false;
        bool LevelThree = false;
        bool LevelFour = false;
};



class FDIR_Master;

class Protocol {
private:
int buffer[7];
int id;
public:
bool isMessageValid(void);
Protocol();

Message receive();
void readData(int id);

};

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



class FDIR_Master {
private:
Board *boards[10];
Executable *schedule[10];
bool reg(Executable* e);

//[interruptPin][boards]
//int BoardIdToInterruptPin[2][2];

volatile int counter_first = 0;
volatile int counter_second = 0;
int anzahlBoards = 0;
public:

FDIR_Master();

void setup_pins();

void searchForAddresses();

void doScheduling();

void setCounterOne(int i);

void setCounterTwo(int i);

int getCounterOne();

int getCounterTwo();


};


#endif
