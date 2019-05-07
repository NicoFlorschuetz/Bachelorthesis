#include <Wire.h>

#include "/Users/nicoflorschutz/Documents/Bachelorthesis/eigeneBib/class.h"

//Id aufbau: ID[0] = Arduino id; ID[1]= fehler oder nicht ; ID[2] = fehler status

int ID[] = {1, 0, 0, 0};
int counter= 0;
int address = 6;
int command;

Pins Arduino1(11, 10, 8,  12, ID);

void setup() {
  
  //Wire.onReceive(receiveEvent);

  
  Serial.begin(9600);
  Wire.begin(address);
  Arduino1.pin_setup();
  
  
}

void loop() {

  digitalWrite(4, LOW);
  delayMicroseconds(1);
  counter++;
  if(counter == 1000){
    digitalWrite(4,HIGH);
    counter = 0;
  }
  Wire.onRequest(requestEvent);
  Arduino1.sensor_reading();
}



void requestEvent()
{
  //switch(command){
    //case 1:  
     //   Wire.write(address);
    //case 2:
        if (Fehlermeldung == 01) {
          Arduino1.ID[1] = 1;
          Arduino1.ID[2] = Arduino1.Fehlercode;
          for (int x = 0; x < Arduino1.ID; x++) {
            Wire.write(Arduino1.ID[x]);
          }
        }else if (Fehlermeldung == 02){
          Arduino1.ID[1] = 1;
          Arduino1.ID[2] = Arduino1.Fehlercode;
          for (int x = 0; x < Arduino1.ID; x++) {
            Wire.write(Arduino1.ID[x]);
          }
        }else{
          Arduino1.ID[1] = 0;
          Arduino1.ID[2] = 0;
          for (int x = 0; x <= Arduino1.ID; x++) {
            if (x == 3){
              Arduino1.ID[x] = realValue;
            }
            Wire.write(Arduino1.ID[x]);
          }
       //}
     
  }
}



void receiveEvent(int bytes) {
        command = Wire.read();
        
}
