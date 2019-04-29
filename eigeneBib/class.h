#include "Wire.h"


#ifndef CLASS_H
#define CLASS_H
boolean ready = false;
unsigned long int startTime = 0;
unsigned long int interval = 1000;
boolean state = false;
int Fehlermeldung;
class Pins {
  private:
    int MegaSim1 = 5;
    int MegaSim2 = 6;
    int licht = 12;
    //int Pin_ID[];
    String incomingMessage;
    boolean resetStatus = false;




  public:
    int Fehlercode = 0;
    int ID[3];
    int keep_alive, resetLED, sign, powerOnOff;
    Pins (int x , int y , int z, int id[3] ) {
      keep_alive = x;
      resetLED = y;
      powerOnOff = z;
      memcpy(ID, id, sizeof(ID));


    };

    void first_setup() {
      pinMode(licht, OUTPUT);
      pinMode(keep_alive, OUTPUT);
      pinMode(resetLED, OUTPUT);
      pinMode(MegaSim1, INPUT);
      pinMode(MegaSim2, INPUT);
      pinMode(powerOnOff, OUTPUT);
      digitalWrite(licht, HIGH);
    };

    static void receiveEvent( int bytes )
    {
      int x = Wire.read(); // read one character from the I2C
      if (x == 1) {
        digitalWrite(8, HIGH);
        delay(1000);
        digitalWrite(8, LOW);
      }
    };

    void first_loop() {
      int reading = analogRead(A0);
      int realValue = map(reading, 0, 1043, 0, 100);
      if (realValue > 70) {
        digitalWrite(licht, LOW);
        Fehlermeldung = 01;
        resetStatus = true;
        Fehlercode = 01;
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

#endif
