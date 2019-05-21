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
        Timer1.initialize(4*1000000);
        Timer1.attachInterrupt(keep_alive);
        Serial.println("Setup main processor end");
}

void loop() {
        boolean PIN = digitalRead(10);
        if(PIN == HIGH) {
                Serial.println(RESET_MODE);
                digitalWrite(11, HIGH);
                digitalWrite(12, LOW);
        }else if(PIN == LOW) {
                Serial.println(NORMAL_MODE);
                digitalWrite(11, LOW);
                digitalWrite(12, HIGH);
        }
        delay(200);
}

void keep_alive(){
        state = !state;
        digitalWrite(4, state);
}
