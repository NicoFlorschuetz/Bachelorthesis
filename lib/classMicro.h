#ifndef classMicro_h
#define classMicro_h

#include "Arduino.h"

//may be change to solveProblem and bool struct with yes or no
/*typedef enum FehlerMeldung {
        EVERYTHING_FINE = 0,
        SIMPLE_FAILURE = 01,
        BIGGER_FAILURE = 02
   }FehlerMeldung_one;*/

struct SolveProblemSuccess {
        bool Solved = false;
};

typedef enum FehlerCode {
        CODE_ZERO = 0,
        CODE_ONE = 1,
        CODE_TWO = 2,
        CODE_THREE = 3,
        CODE_FOUR = 4
}FehlerCode_one;

extern unsigned long int interval1;
extern unsigned long int interval2;
extern unsigned int realValue;
extern unsigned long current1;


class FDIR_Slave {
private:

//FehlerMeldung Fehlermeldung;
FehlerCode Fehlercode;
String incomingMessage;
bool resetStatus = false;
int _resetLED, _sign, _powerOnOff, _licht, _address;

public:
SolveProblemSuccess solveProblemSuccess;
int ID[4];
FDIR_Slave(int y, int z, int sensor, int id[4], int address);

int getFailure();

int getFailurecode();

void board_setup();

void sensor_reading();


};
#endif
