#include <Wire.h>

class Pins {
  private:
    int keep_alive, resetLED, sign, powerOnOff;
    int MegaSim1 = 5;
    int MegaSim2 = 6;
    String incomingMessage;
    boolean debug, resetARD, power;
    unsigned long int startTime = 0;
    unsigned long int interval = 1000;
    boolean state = false;


  public:
    Pins (int x , int y , int z ,  boolean deb, boolean ARD) {
      keep_alive = x;
      resetLED = y;
      powerOnOff = z;
      debug = deb;
      resetARD = ARD;
    };



    void setup() {
      pinMode(keep_alive, OUTPUT);
      pinMode(resetLED, OUTPUT);
      pinMode(MegaSim1, INPUT);
      pinMode(MegaSim2, INPUT);
      pinMode(powerOnOff, OUTPUT);
      //Serial.println("Setup end");
    };

   

    void loop() {
      long current = millis();
      if (current - startTime >= interval) {
        state = !state;
        digitalWrite(keep_alive, state);
        startTime = millis();
      }
      
      if (digitalRead(MegaSim1) == true) {
        //Serial.println("reset start");
        //digitalWrite(keep_alive, LOW);
        digitalWrite(resetLED, HIGH);
        delay(2000);
        digitalWrite(resetLED, LOW);
        //Serial.println("reset end");
        delay(200);
      } else if (digitalRead(MegaSim2) == true) {
        digitalWrite(powerOnOff, HIGH);
        digitalWrite(resetLED, LOW);
        digitalWrite(keep_alive, LOW);
        delay(2000);
        digitalWrite(powerOnOff, LOW);
        power = false;
        debug = true;
      }


    };
};

Pins Arduino1(11, 10, 8, true, false);

void setup() {
  // put your setup code here, to run once:
  Arduino1.setup();
}

void loop() {

  // put your main code here, to run repeatedly:
  Arduino1.loop();
}
