#include "Wire.h"
#ifndef CLASS_H
#define CLASS_H



unsigned long int startTime = 0;
unsigned long int interval = 2000;
unsigned long int startTime1 = 0;
unsigned long int interval1 = 3000;
unsigned long int interval2 = 1000;
boolean state = false;
int realValue;
int Fehlermeldung;
long current1 = 0;

class Pins {
private:

int licht;
//int Pin_ID[];
String incomingMessage;
boolean resetStatus = false;





public:
int Fehlercode = 0;
int ID[4];

int keep_alive, resetLED, sign, powerOnOff;
Pins (int x, int y, int z, int sensor, int id[4] ) {
        keep_alive = x;
        resetLED = y;
        powerOnOff = z;

        licht = sensor;
        memcpy(ID, id, sizeof(ID));


};



void pin_setup() {
        pinMode(licht, OUTPUT);
        pinMode(keep_alive, OUTPUT);
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
                        Fehlermeldung = 02;
                        resetStatus = true;
                        Fehlercode = 2;
                }else if (current1 >= interval2) {
                        Fehlermeldung = 01;
                        Fehlercode = 1;
                }
        }else{
                current1 = 0;
                Fehlercode = 0;
                Fehlermeldung = 00;
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
