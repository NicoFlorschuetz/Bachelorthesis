#include "Arduino.h"
#include "Wire.h"
#include "classMicro.h"

unsigned long int interval1 = 3000;
unsigned long int interval2 = 1000;
unsigned int realValue = 0;
unsigned long current1 = 0;

Pins::Pins(int y, int z, int sensor, int id[4]){
        _resetLED = y;
        _powerOnOff = z;
        _licht = sensor;
        memcpy(ID, id, sizeof(ID));
}

void Pins::pin_setup(){
        pinMode(_licht, OUTPUT);
        pinMode(_resetLED, OUTPUT);
        pinMode(_powerOnOff, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(_licht, HIGH);
}

int Pins::getFehlermeldung(){
        return Fehlermeldung;
}

int Pins::getFehlercode(){
        return Fehlercode;
}

void Pins::sensor_reading(){
        int reading = analogRead(A0);
        realValue = map(reading, 0, 1000, 0, 100);
        if (realValue > 70) {
                current1 += 1;
                if(current1 >= interval1) {
                        digitalWrite(_licht, LOW);
                        Fehlermeldung = BIGGER_FAILURE;
                        resetStatus = true;
                        Fehlercode = CODE_TWO;
                }else if (current1 >= interval2) {
                        Fehlermeldung  = SIMPLE_FAILURE;
                        Fehlercode = CODE_ONE;
                }
        }else{
                current1 = 0;
                Fehlercode = CODE_ZERO;
                Fehlermeldung = EVERYTHING_FINE;
        }

        Serial.println(realValue);
        if ( resetStatus == true) {
                digitalWrite(_resetLED, HIGH);
                delay(4000);
                digitalWrite(_resetLED, LOW);
                resetStatus = false;
                digitalWrite(_licht, HIGH);
        } /*else if ( == true ) {
                digitalWrite(_powerOnOff, HIGH);
                digitalWrite(_resetLED, LOW);
                digitalWrite(_keep_alive, LOW);
                delay(2000);
                digitalWrite(_powerOnOff, LOW);


             }*/
}