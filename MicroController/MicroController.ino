#include <Wire.h>



boolean ready = false;
unsigned long int startTime = 0;
unsigned long int interval = 1000;
boolean state = false;
String Fehlermeldung;





class Pins {
  private:
    int MegaSim1 = 5;
    int MegaSim2 = 6;
    int licht = 12;
    String incomingMessage;
    boolean resetARD, power;
    boolean resetStatus = false;



  public:
    int keep_alive, resetLED, sign, powerOnOff;
    Pins (int x , int y , int z ,   boolean ARD) {
      keep_alive = x;
      resetLED = y;
      powerOnOff = z;
      resetARD = ARD;
    };

    void setup() {
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

    void loop() {
      int reading = analogRead(A0);
      int realValue = map(reading, 0, 1043, 0, 100);
      if (realValue > 70) {
        digitalWrite(licht, LOW);
        Fehlermeldung = "Zu viel Licht";
        resetStatus = true;
      }else{
        Fehlermeldung = " ";
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
        power = false;

      }
    };
};

Pins Arduino1(11, 10, 8, false);

void setup() {
  Serial.begin(9600);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Arduino1.setup();
}

void loop() {
  if ( ready == true) {
    long current = millis();
    if (current - startTime >= interval ) {
      state = !state;
      digitalWrite(Arduino1.keep_alive, state);
      Wire.onRequest(requestEvent);
      startTime = millis();
    }
    Arduino1.loop();
  }
}

void receiveEvent(int bytes) {
  int x = Wire.read();
  if (x == 1) {
    ready = true;
  }
}

void requestEvent()
{
  if (Fehlermeldung == "Zu viel Licht"){
    Wire.write("Error");
  }else{
    Wire.write("alles gut");
  }
 
}
