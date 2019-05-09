#include "Arduino.h"
#include "Wire.h"
#include "classMega.h"

Simulator::Simulator(int a, int b){
        _a = a;
        _b = b;
}

void Simulator::setup_pins(){
        pinMode(RESET_ARDUINO1, OUTPUT);
        pinMode(RESET_ARDUINO2, OUTPUT);
        pinMode(PIN_FIRST, INPUT);
        pinMode(PIN_SECOND, INPUT);
        digitalWrite(RESET_ARDUINO1, HIGH);
        digitalWrite(RESET_ARDUINO2, HIGH);
}

void Simulator::setCounterOne(int i){
        if (i == 1) {
                counter_first += i;
        }else if(i==0) {
                counter_first = 0;
        }
}

void Simulator::setCounterTwo(int i){
        if(i == 1) {
                counter_second += i;
        }else if(i == 0) {
                counter_second = 0;
        }
}

int Simulator::getCounterOne(){
        return counter_first;
}

int Simulator::getCounterTwo(){
        return counter_second;
}

int Simulator::getPins_Arduino(){
        return Pins_Arduino;
}

void Simulator::failure_detection(int x, int nodeAddress){
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
                                digitalWrite(RESET_ARDUINO1, LOW);
                                delay(2000);
                                digitalWrite(RESET_ARDUINO2, HIGH);
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
}

void Simulator::serial_input(){
        if (Serial.available()) {
                Message = Serial.readString();
                Message.trim();
                if ( Message == "reset board 1") {
                        Serial.println("reset start");
                        digitalWrite(RESET_ARDUINO1, LOW);
                        delay(2000);
                        digitalWrite(RESET_ARDUINO1, HIGH);
                        Serial.println("reset end");
                }else if (Message == "reset board 2") {
                        Serial.println("reset start");
                        digitalWrite(RESET_ARDUINO2, LOW);
                        delay(2000);
                        digitalWrite(RESET_ARDUINO2, HIGH);
                        Serial.println("reset end");
                }else if (Message == "shut down board 1") {
                        digitalWrite(RESET_ARDUINO1, LOW);
                }else if (Message =="turn on board 1") {
                        digitalWrite(RESET_ARDUINO1, HIGH);
                }else if (Message == "shut down board 2") {
                        digitalWrite(RESET_ARDUINO2, LOW);
                }else if (Message =="turn on board 2") {
                        digitalWrite(RESET_ARDUINO2, HIGH);
                }
                Serial.println("I received: " + Message );

        }
}

void Simulator::search(){
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

void Simulator::address_range(){
        for(int i = 0; i < sizeof(nodeAddress)/sizeof(nodeAddress[0]); i++) {
                Wire.requestFrom(nodeAddress[i], 7);
                delay(500);
                for (int x = 0; x <= Wire.available(); x++) {
                        antwort[x] = Wire.read();
                        failure_detection(x, nodeAddress[i]);
                }
        }
}
