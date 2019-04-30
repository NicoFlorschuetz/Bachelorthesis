#include "Wire.h"
#ifndef CLASS_H
#define CLASS_H


boolean ready = false;
unsigned long int startTime = 0;
unsigned long int interval = 1000;
boolean state = false;
int realValue;
int Fehlermeldung;

class Pins {
private:
int MegaSim1;
int MegaSim2;
int licht;
//int Pin_ID[];
String incomingMessage;
boolean resetStatus = false;




public:
int Fehlercode = 0;
int ID[4];
int keep_alive, resetLED, sign, powerOnOff;
Pins (int x, int y, int z, int r, int w, int sensor, int id[4] ) {
        keep_alive = x;
        resetLED = y;
        powerOnOff = z;
        MegaSim1 = r;
        MegaSim2 = w;
        licht = sensor;
        memcpy(ID, id, sizeof(ID));


};

void first_setup() {
        Serial.begin(9600);
        Wire.begin(10);
        pinMode(licht, OUTPUT);
        pinMode(keep_alive, OUTPUT);
        pinMode(resetLED, OUTPUT);
        pinMode(MegaSim1, INPUT);
        pinMode(MegaSim2, INPUT);
        pinMode(powerOnOff, OUTPUT);
        digitalWrite(licht, HIGH);
};


void first_loop() {
        int reading = analogRead(A0);
        realValue = map(reading, 0, 1000, 0, 100);
        if (realValue > 70) {
                digitalWrite(licht, LOW);
                Fehlermeldung = 01;
                resetStatus = true;
                Fehlercode = 1;
        } else {
                Fehlermeldung = 0;
                Fehlercode = 0;
        }

        Serial.print(reading);
        Serial.print("\t");
        Serial.println(realValue);

        if (digitalRead(MegaSim1) == true or resetStatus == true) {
                digitalWrite(resetLED, HIGH);
                delay(4000);
                digitalWrite(resetLED, LOW);
                resetStatus = false;
                digitalWrite(licht, HIGH);
        } else if (digitalRead(MegaSim2) == true ) {
                digitalWrite(powerOnOff, HIGH);
                digitalWrite(resetLED, LOW);
                digitalWrite(keep_alive, LOW);
                delay(2000);
                digitalWrite(powerOnOff, LOW);

        }
};
};

void receiveEvent(int bytes) {
        int x = Wire.read();
        if (x == 1) {
                ready = true;
        } else {
                ready = false;
        }
}




#endif
