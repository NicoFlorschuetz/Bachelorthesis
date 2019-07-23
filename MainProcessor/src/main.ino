#include <Arduino.h>
#include "TimerOne.h"

typedef enum Modes {
        RESET_MODE,
        NORMAL_MODE,
        BOOT_MODE
}my_Mode;

boolean state = false;

void setup() {
        Serial.begin(9600);
        Serial.println("Setup main processor start");
        while (!Serial);
        pinMode(10, INPUT);
        pinMode(11,OUTPUT);
        pinMode(12,OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(11,LOW);
        digitalWrite(4,LOW);
        Timer1.initialize(4*1000000);
        Timer1.attachInterrupt(keep_alive);
        Serial.println("Setup main processor end");
}

void loop() {
        boolean PIN = digitalRead(10);
        if(PIN == HIGH) {
                digitalWrite(11, HIGH);
                digitalWrite(12, LOW);
                delay(2000);
        }else if(PIN == LOW) {
                digitalWrite(11, LOW);
                digitalWrite(12, HIGH);
        }
}

void keep_alive(){
        digitalWrite(4,HIGH);
        digitalWrite(4,LOW);
}
