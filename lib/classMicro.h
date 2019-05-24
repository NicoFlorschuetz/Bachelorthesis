#ifndef classMicro_h
#define classMicro_h

#include "Arduino.h"


struct ProblemStatus {
        bool Normal = false;
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

FehlerCode FailureCode;
String incomingMessage;

int _resetLED, _sign, _powerOnOff, _licht, _address;

public:
bool resetStatus = false;
ProblemStatus problemStatus;
int ID[4];
FDIR_Slave(int y, int z, int sensor, int id[4], int address);

int getFailurecode();

void board_setup();

void sensor_reading();

};
#endif
