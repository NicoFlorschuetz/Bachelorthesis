#include "Wire.h"
#ifndef CLASSMEGA_H
#define CLASSMEGA_H

class Simulator {
  private:
    int LEDreset, LEDpower;
    boolean reset, power;
    String Message;
    int antwort[3];

  public:
    Simulator(int a, int b, boolean res, boolean pow) {
      LEDreset = a;
      LEDpower = b;
      reset = res;
      power = pow;
    };

    void setup() {
      Wire.begin();
      Serial.begin(9600);
      while (!Serial);
      Serial.println("Setup begin");
      pinMode(LEDreset, OUTPUT);
      pinMode(LEDpower, OUTPUT);
      Serial.println("Setup end");

    };


    void loop() {

      Wire.beginTransmission( 9 ); // transmit to device #9
      Wire.write(1); // sends x
      Wire.endTransmission(); // stop transmitting
      Wire.requestFrom(9, 4);
      delay(1000);
      for (int x = 0; x <= Wire.available(); x++) {
        antwort[x] = Wire.read();
        Serial.print(antwort[x]);
      }
      Serial.print("\n");

      if (Serial.available()) {
        Message = Serial.readString();
        Message.trim();
        if ( Message == "reset") {
          reset = true;
          power = false;
        } else if (Message == "power") {
          reset = false;
          power = true;
        }
        Serial.println("I received: " + Message );
      }


      if (reset) {
        Serial.println("reset start");
        digitalWrite(LEDreset, HIGH);
        delay(2000);
        digitalWrite(LEDreset, LOW);
        Serial.println("reset end");
        reset = false;
        power = false;
      } else if (power) {
        Serial.println("power");
        digitalWrite(LEDpower, HIGH);
        delay(2000);
        digitalWrite(LEDpower, LOW);
        power = false;
        reset = false;

      }

    };

};

#endif