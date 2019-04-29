#include <Wire.h>
#include "/Users/nicoflorschutz/Documents/Bachelorthesis/eigeneBib/class.h"

//Id aufbau: ID[0] = Arduino id; ID[1]= fehler oder nicht ; ID[2] = fehler status
int ID[] = {1, 0, 0};
Pins Arduino1(11, 10, 8,5,6,12, ID);

void setup() {
  Serial.begin(9600);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Arduino1.first_setup();
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
    Arduino1.first_loop();
  }
}

void receiveEvent(int bytes) {
  int x = Wire.read();
  if (x == 1) {
    ready = true;
  }else{
    ready = false;
  }
}

void requestEvent()
{

  if (Fehlermeldung == 01) {
    Arduino1.ID[1] = 1;
    Arduino1.ID[2] = Arduino1.Fehlercode;
    for (int x = 0; x < Arduino1.ID; x++) {
      Wire.write(Arduino1.ID[x]);
    }
   

  } else {
    Arduino1.ID[1] = 0;
    Arduino1.ID[2] = 0;
    for (int x = 0; x < Arduino1.ID; x++) {
      Wire.write(Arduino1.ID[x]);
    }
  }

}
