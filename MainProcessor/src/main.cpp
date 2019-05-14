#include <Arduino.h>

typedef enum Modes {
        RESET_MODE,
        NORMAL_MODE,
        BOOT_MODE
}my_Mode;

void setup() {
        Serial.begin(9600);
        Serial.println("Setup main processor start");
        while (!Serial);
        pinMode(10, INPUT);
        pinMode(11,OUTPUT);
        pinMode(12,OUTPUT);
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
        delay(500);
}
