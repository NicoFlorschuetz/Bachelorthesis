#ifndef classMega_h
#define classMega_h

#include "Arduino.h"
#include <Wire.h>

typedef enum pins_arduino {
        PIN_FIRST = 2,
        PIN_SECOND = 3,
        RESET_ARDUINO1 = 9,
        RESET_ARDUINO2 = 8,

}pins_arduino_t;

class Simulator {
private:
pins_arduino Pins_Arduino;
String Message;
int antwort[7];
int counter_of_keep = 0;
int nodeAddress[2];
int _a,_b;
volatile int counter_first = 0;
volatile int counter_second = 0;

public:
Simulator(int a, int b);

void setup_pins();

void setCounterOne(int i);

void setCounterTwo(int i);

int getCounterOne();

int getCounterTwo();

int getPins_Arduino();

void failure_detection(int x, int nodeAddress);

void serial_input();

void search();

void address_range();

};

#endif
