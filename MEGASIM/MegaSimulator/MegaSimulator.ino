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
      Wire.begin();
      Serial.begin(9600);
      while (!Serial);
      Serial.println("Setup begin");
      pinMode(LEDreset, OUTPUT);
      pinMode(LEDpower, OUTPUT);
      Serial.println("Setup end");
      Wire.beginTransmission( 9 ); // transmit to device #9
      Wire.write(1); // sends x
      Wire.endTransmission(); // stop transmitting
    };


    void loop() {
      Wire.requestFrom(9, 9);
      delay(1000);
      while (Wire.available()) {
        char antwort = Wire.read();
        Serial.print(antwort);
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

Simulator Mega1(8, 9);

void setup() {
  // put your setup code here, to run once:
  Mega1.setup();
}

void loop() {

  // put your main code here, to run repeatedly:
  Mega1.loop();
}
