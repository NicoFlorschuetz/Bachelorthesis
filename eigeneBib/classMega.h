#include "Wire.h"
#ifndef CLASSMEGA_H
#define CLASSMEGA_H

class Simulator {
private:
int pin_first = 2;
int pin_second = 3;
int LEDreset1, LEDreset2;
String Message;
int antwort[7];
int counter_of_keep = 0;
int nodeAddress[2];

public:
volatile int counter_first = 0;
volatile int counter_second = 0;
Simulator(int a, int b) {
        LEDreset1 = a;
        LEDreset2 = b;

};

void setup_pins(){
        pinMode(LEDreset1, OUTPUT);
        pinMode(LEDreset2, OUTPUT);
        pinMode(pin_first, INPUT);
        pinMode(pin_second, INPUT);
        digitalWrite(LEDreset1, HIGH);
        digitalWrite(LEDreset2, HIGH);
};

void failure_detection(int x, int nodeAddress){
        if (antwort[0] != -1) {
                if ( x >=3 ) {
                        Serial.print("The value of board ");
                        Serial.print(antwort[0]);
                        Serial.print(" is: ");
                        Serial.println(antwort[x]);
                }else if(antwort[1] == 1  && x>1) {
                        if (antwort[2] == 1) {
                                Serial.println("Simple failure");
                                Serial.print(antwort[1]);
                                Serial.println(antwort[2]);
                        }else if (antwort[2]== 2) {
                                Serial.println("failure is bigger");
                                Serial.print(antwort[1]);
                                Serial.println(antwort[2]);
                                digitalWrite(LEDreset1, LOW);
                                delay(2000);
                                digitalWrite(LEDreset1, HIGH);
                        }
                }else if(antwort[0] == -1) {
                        Serial.println("No connection, try to reconnect");
                        //address_range();
                }
        }else{
                Serial.print("Board ");
                Serial.print(nodeAddress);
                Serial.println(" ist down");
        }
        serial_input();
        //Serial.print("\n");
};

void serial_input(){
        if (Serial.available()) {
                Message = Serial.readString();
                Message.trim();
                if ( Message == "reset board 1") {
                        Serial.println("reset start");
                        digitalWrite(LEDreset1, LOW);
                        delay(2000);
                        digitalWrite(LEDreset1, HIGH);
                        Serial.println("reset end");
                }else if (Message == "reset board 2") {
                        Serial.println("reset start");
                        digitalWrite(LEDreset2, LOW);
                        delay(2000);
                        digitalWrite(LEDreset2, HIGH);
                        Serial.println("reset end");
                }else if (Message == "shut down board 1") {
                        digitalWrite(LEDreset1, LOW);
                }else if (Message =="turn on board 1") {
                        digitalWrite(LEDreset1, HIGH);
                }else if (Message == "shut down board 2") {
                        digitalWrite(LEDreset2, LOW);
                }else if (Message =="turn on board 2") {
                        digitalWrite(LEDreset2, HIGH);
                }
                Serial.println("I received: " + Message );

        }
};

void search(){
        int zaehler = 0;
        for(byte search_address = 1; search_address < 120; search_address++) {

                Wire.beginTransmission(search_address);
                if(Wire.endTransmission() == 0) {
                        nodeAddress[zaehler] = search_address;
                        zaehler++;
                        Serial.println(search_address);
                }
                delay(5);
        }
        zaehler = 0;
}

void address_range() {

        for(int i = 0; i < 2; i++) {
                Wire.requestFrom(nodeAddress[i], 7);
                delay(500);
                for (int x = 0; x <= Wire.available(); x++) {
                        antwort[x] = Wire.read();
                        failure_detection(x, nodeAddress[i]);
                }
        }
};

};

#endif
