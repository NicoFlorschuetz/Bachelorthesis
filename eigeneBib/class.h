#include <Wire.h>
#ifndef CLASS_H
#define CLASS_H

enum FehlerMeldung {
        EVERYTHING_FINE = 0,
        SIMPLE_FAILURE = 01,
        BIGGER_FAILURE = 02
};

enum FehlerCode {
        CODE_ZERO = 0,
        CODE_ONE = 1,
        CODE_TWO = 2
};

unsigned long int interval1 = 3000;
unsigned long int interval2 = 1000;
int realValue = 0;
long current1 = 0;


class Pins {
private:
FehlerMeldung Fehlermeldung;
FehlerCode Fehlercode;
String incomingMessage;
bool resetStatus = false;
int resetLED, sign, powerOnOff, licht;

public:
int ID[4];
Pins ( int y, int z, int sensor, int id[4] ) {
        resetLED = y;
        powerOnOff = z;
        licht = sensor;
        memcpy(ID, id, sizeof(ID));
};



getFehlermeldung (){
        return Fehlermeldung;
};

getFehlercode (){
        return Fehlercode;
};

void pin_setup() {
        pinMode(licht, OUTPUT);
        pinMode(resetLED, OUTPUT);
        pinMode(powerOnOff, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(licht, HIGH);
};

void sensor_reading() {

        int reading = analogRead(A0);
        realValue = map(reading, 0, 1000, 0, 100);
        if (realValue > 70) {
                current1 += 1;
                if(current1 >= interval1) {
                        digitalWrite(licht, LOW);
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
                digitalWrite(resetLED, HIGH);
                delay(4000);
                digitalWrite(resetLED, LOW);
                resetStatus = false;
                digitalWrite(licht, HIGH);
        } /*else if ( == true ) {
                digitalWrite(powerOnOff, HIGH);
                digitalWrite(resetLED, LOW);
                digitalWrite(keep_alive, LOW);
                delay(2000);
                digitalWrite(powerOnOff, LOW);

             }*/
};
};
#endif
