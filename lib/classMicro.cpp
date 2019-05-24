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
        pinMode(2, INPUT);
        digitalWrite(_licht, HIGH);
}


int FDIR_Slave::getFailurecode(){
        return FailureCode;
}

void FDIR_Slave::sensor_reading(){
        int reading = analogRead(A0);
        realValue = map(reading, 0, 1000, 0, 100);
        if (realValue > 70) {
                current1 += 1;
                if(current1 >= interval1 && problemStatus.Normal == false) {
                        if (tryRestart >= 1) {
                                FailureCode = CODE_THREE;
                        }else{
                                digitalWrite(_licht, LOW);

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

        Serial.println(realValue);

        if ( resetStatus == true) {
                digitalWrite(_resetLED, HIGH);
                digitalWrite(_resetLED, LOW);
                delay(500);
                resetStatus = false;
                digitalWrite(_licht, HIGH);
        }

}
