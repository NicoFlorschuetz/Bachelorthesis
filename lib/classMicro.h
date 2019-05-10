#ifndef classMicro_h
#define classMicro_h

#include "Arduino.h"


typedef enum FehlerMeldung {
        EVERYTHING_FINE = 0,
        SIMPLE_FAILURE = 01,
        BIGGER_FAILURE = 02
}FehlerMeldung_one;

typedef enum FehlerCode {
        CODE_ZERO = 0,
        CODE_ONE = 1,
        CODE_TWO = 2
}FehlerCode_one;

extern unsigned long int interval1;
extern unsigned long int interval2;
extern unsigned int realValue;
extern unsigned long current1;


class Pins {
private:
FehlerMeldung Fehlermeldung;
FehlerCode Fehlercode;
String incomingMessage;
bool resetStatus = false;
int _resetLED, _sign, _powerOnOff, _licht, _address;

public:
int ID[4];
Pins(int y, int z, int sensor, int id[4], int address);

int getFehlermeldung();

int getFehlercode();

void board_setup();

void sensor_reading();

void send_keep_alive_signal(int counter);

};
#endif
