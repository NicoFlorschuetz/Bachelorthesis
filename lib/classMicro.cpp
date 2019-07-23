#include "Arduino.h"
#include "Wire.h"
#include "classMicro.h"

unsigned long int interval1 = 2000;
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
        while (!Serial);
        pinMode(_licht, OUTPUT);
        pinMode(_resetLED, OUTPUT);
        pinMode(_powerOnOff, OUTPUT);
        pinMode(3, INPUT);
        pinMode(2, INPUT);
        digitalWrite(_licht, HIGH);
        digitalWrite(_resetLED, LOW);
        digitalWrite(_powerOnOff, LOW);
}

void FDIR_Slave::sensor_reading(){
        int reading = analogRead(A0);
        realValue = map(reading, 0, 1024, 0, 100);
        if (realValue >= 70 || boardIsOkay == false) {
                current1 += 1;
                if((current1 >= interval1 && problemStatus.Normal == false) || boardIsOkay == false) {
                        if (tryRestart >= 3) {
                                FailureCode = CODE_THREE;
                        }else{
                                resetStatus = true;
                                FailureCode = CODE_TWO;
                                tryRestart++;
                        }
                }else if (current1 >= interval2) {
                        problemStatus.Normal = false;
                        FailureCode = CODE_ONE;
                }
        }else{
                tryRestart = 0;
                problemStatus.Normal = true;
                current1 = 0;
                FailureCode = CODE_ZERO;
                digitalWrite(_licht, HIGH);
        }
        delay(5);
        if ( resetStatus == true || boardIsOkay == false) {
                digitalWrite(_resetLED, HIGH);
                digitalWrite(_resetLED, LOW);
                delay(500);
                resetStatus = false;
        }

}

int FDIR_Slave::getFailurecode(){
        return FailureCode;
}

void FDIR_Slave::setCounter(int i){
        if (i == 1) {
                counter_first += i;
        }else if(i == 0) {
                counter_first = 0;
        }
}

int FDIR_Slave::getCounter(){
        return counter_first;
}
