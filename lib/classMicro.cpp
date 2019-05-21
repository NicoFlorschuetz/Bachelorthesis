#include "Arduino.h"
#include "Wire.h"
#include "classMicro.h"

unsigned long int interval1 = 3000;
unsigned long int interval2 = 1000;
unsigned int realValue = 0;
unsigned long current1 = 0;
int tryRestart = 0;

FDIR_Slave::FDIR_Slave(int y, int z, int sensor, int id[4], int address){
        _resetLED = y;
        _powerOnOff = z;
        _licht = sensor;
        memcpy(ID, id, sizeof(ID));
        _address = address;
}

void FDIR_Slave::board_setup(){
        Serial.begin(9600);
        Wire.begin(_address);
        pinMode(_licht, OUTPUT);
        pinMode(_resetLED, OUTPUT);
        pinMode(_powerOnOff, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(_licht, HIGH);
}

int FDIR_Slave::getFailure(){
        //  return Fehlermeldung;
}

int FDIR_Slave::getFailurecode(){
        return Fehlercode;
}

void FDIR_Slave::sensor_reading(){
        int reading = analogRead(A0);
        realValue = map(reading, 0, 1000, 0, 100);
        if (realValue > 70) {
                current1 += 1;
                if(current1 >= interval1) {
                        digitalWrite(_licht, LOW);

                        //Fehlermeldung = BIGGER_FAILURE;
                        resetStatus = true;
                        Fehlercode = CODE_TWO;

                        if (tryRestart >= 1) {
                                solveProblemSuccess.Solved = false;
                                tryRestart == 0;
                        }
                        tryRestart++;
                }else if (current1 >= interval2) {
                        solveProblemSuccess.Solved = true;
                        //Fehlermeldung  = SIMPLE_FAILURE;
                        Fehlercode = CODE_ONE;
                }
        }else{
                solveProblemSuccess.Solved = true;
                current1 = 0;
                Fehlercode = CODE_ZERO;
                //Fehlermeldung = EVERYTHING_FINE;
        }

        Serial.println(realValue);
        if ( resetStatus == true) {
                digitalWrite(_resetLED, HIGH);
                delay(4000);
                digitalWrite(_resetLED, LOW);
                resetStatus = false;
                digitalWrite(_licht, HIGH);
        }
}
