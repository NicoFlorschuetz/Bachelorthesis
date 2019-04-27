#include <Wire.h>


class Simulator {
  private:
    int LEDreset, LEDpower;
    boolean reset, power;
    String Message;

  public:
    Simulator(int a, int b) {
      LEDreset = a;
      LEDpower = b;
    };

    void setup() {
      Serial.begin(9600);
      while (!Serial);
      Serial.println("Setup begin");
      pinMode(LEDreset, OUTPUT);
      pinMode(LEDpower, OUTPUT);
      Serial.println("Setup end");
    };


    void loop() {
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

Simulator Mega1(8, 9);

void setup() {
  // put your setup code here, to run once:
  Mega1.setup();
}

void loop() {

  // put your main code here, to run repeatedly:
  Mega1.loop();
}
