#include "Arduino.h"
#include "Wire.h"
#include "classMega.h"



Simulator::Simulator(int a, int b){
        _a = a;
        _b = b;
}

void Simulator::setup_pins(){
        digitalWrite(RESET_ARDUINO1, HIGH);
        digitalWrite(RESET_ARDUINO2, HIGH);
        pinMode(RESET_ARDUINO1, OUTPUT);
        pinMode(RESET_ARDUINO2, OUTPUT);
        pinMode(PIN_FIRST, INPUT);
        pinMode(PIN_SECOND, INPUT);
}



void Simulator::setCounterOne(int i){
        if (i == 1) {
                counter_first += i;
        }else if(i == 0) {
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
void Simulator::show_values(int x){
        Serial.print("The value of board ");
        Serial.print(antwort[0]);
        Serial.print(" is: ");
        Serial.println(antwort[x]);
}

void Simulator::print_failure(String Failure_Message){
        Serial.print(Failure_Message);
        Serial.print(antwort[1]);
        Serial.println(antwort[2]);
}

void Simulator::reset_boards(){
        switch (antwort[0]) {
        case 1:
                digitalWrite(RESET_ARDUINO1, LOW);
                delay(2000);
                digitalWrite(RESET_ARDUINO1, HIGH);
                counter_failure_board1++;
                break;
        case 2:
                digitalWrite(RESET_ARDUINO2, LOW);
                delay(2000);
                digitalWrite(RESET_ARDUINO2, HIGH);
                counter_failure_board2++;
                break;
        default:
                break;
        }

}

void Simulator::failure_detection(int x, int nodeAddress){
        if (antwort[0] != -1) {
                if ( x >=3 ) {
                        show_values(x);
                }else if(antwort[1] == 1  && x>1) {
                        switch (antwort[2]) {
                        case 1:
                                print_failure("Simple failure: ");
                                break;
                        case 2:
                                print_failure("Big failure: ");
                                reset_boards();
                                if (counter_failure_board1 > 2) {
                                        Serial.println("Error could not be fixed, switch to redundant board");
                                        counter_failure_board1 = 0;
                                        digitalWrite(RESET_ARDUINO1, LOW);
                                }else if(counter_failure_board2 > 2) {
                                        Serial.println("Error could not be fixed, switch to redundant board");
                                        counter_failure_board2 = 0;
                                        digitalWrite(RESET_ARDUINO2, LOW);
                                }
                                break;
                        default:
                                break;
                        }
                }
        }else if(antwort[0] == -1) {
                Serial.println("No connection, try to reconnect");
                //address_range();

        }else{
                Serial.print("Board with address");
                Serial.print(nodeAddress);
                Serial.println(" ist down");
        }
        serial_input();
}

void Simulator::serial_input(){
        if (Serial.available()) {
                Message = Serial.readString();
                Message.trim();
                switch (Message) {
                case "reset board 1":
                        Serial.println("reset start");
                        digitalWrite(RESET_ARDUINO1, LOW);
                        delay(2000);
                        digitalWrite(RESET_ARDUINO1, HIGH);
                        Serial.println("reset end");
                        break;
                case "reset board 2":
                        Serial.println("reset start");
                        digitalWrite(RESET_ARDUINO2, LOW);
                        delay(2000);
                        digitalWrite(RESET_ARDUINO2, HIGH);
                        Serial.println("reset end");
                        break;
                case "shut down board 1":
                        digitalWrite(RESET_ARDUINO1, LOW);
                        break;
                case "turn on board 1":
                        digitalWrite(RESET_ARDUINO1, HIGH);
                        break;
                case "shut down board 2":
                        digitalWrite(RESET_ARDUINO2, LOW);
                        break;
                case "turn on board 2":
                        digitalWrite(RESET_ARDUINO2, HIGH);
                        break;
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
                }
                delay(5);
        }
}

void Simulator::address_range(){
        for(int i = 0; i < sizeof(nodeAddress)/sizeof(nodeAddress[0]); i++) {
                Wire.requestFrom(nodeAddress[i], 7);
                delay(500);
                int x = 0;
                while(x<=Wire.available()) {
                        antwort[x] = Wire.read();
                        failure_detection(x, nodeAddress[i]);
                        x++;
                }
        }
}
