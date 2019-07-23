#include <TimerOne.h>
#include <simulation.h>

FDIR_Master Mega;

void setup() {
        Serial.begin(115200);
        Serial.println("Setup begin");
        Wire.begin();
        while (!Serial);
        Mega.searchForAddresses();
        Serial.println("Setup end");
}

void loop() {
        Mega.doScheduling();
        delay(100);
}
