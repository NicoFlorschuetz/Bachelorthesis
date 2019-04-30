#include "Wire.h"
#ifndef CLASSMEGA_H
#define CLASSMEGA_H

class Simulator {
private:
int LEDreset, LEDpower;
boolean reset, power;
String Message;
int antwort[7];
unsigned long int startTime = 0;
unsigned long int interval = 2000;

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
        //long current = millis();
START:
        Wire.beginTransmission( 9 ); // transmit to device #9
        Wire.write(1); // sends x
        Wire.endTransmission(); // stop transmitting
        Wire.requestFrom(9, 7, true);
        delay(500);

        for (int x = 0; x <= Wire.available(); x++) {
                antwort[x] = Wire.read();
                if (antwort[0] != -1) {
                        if (x==0 ) {
                                Serial.print("keep alive Arduino ");
                                Serial.println(antwort[0]);
                        }else if ( x >=3 ) {
                                Serial.print(antwort[ x]);
                        }else if(antwort[1] == 1  && x>1) {
                                Serial.print("We've got an error ");
                                Serial.print(antwort[1]);
                                Serial.println(antwort[2]);
                                continue;
                        }else if(antwort[0] == -1) {
                                Serial.println("No connection, try to reconnect");
                                goto START;
                        }
                }else{
                        Serial.println("Board 1 ist down");
                }
        }

        Serial.print("\n");
        /*memset(antwort, 0, 7);
           Serial.print(antwort[0]);*/
START1:

        //if (current - startTime >= interval ) {
        Wire.beginTransmission( 10 );         // transmit to device #9
        Wire.write(1);         // sends x
        Wire.endTransmission();         // stop transmitting

        Wire.requestFrom(10, 7, true);

        delay(500);

        //if (Wire.available()) {
        for (int x = 0; x <= Wire.available(); x++) {
                antwort[x] = Wire.read();
                if (antwort[0] != -1) {
                        if (x==0 ) {
                                Serial.print("keep alive Arduino ");
                                Serial.println(antwort[0]);
                        }else if ( x >=3 ) {
                                Serial.print(antwort[ x]);
                        }else if(antwort[1] == 1  && x>1) {
                                Serial.print("We've got an error ");
                                Serial.print(antwort[1]);
                                Serial.println(antwort[2]);
                                continue;
                        }else if(antwort[0] == -1) {
                                Serial.println("No connection, try to reconnect");
                                goto START1;
                        }
                }
                else{
                        Serial.println("Board 2 ist down");
                }
        }

        Serial.print("\n");
        //}

        //startTime = millis();

        //}




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
