#include <TimerOne.h>
#include <classMega.h>


Simulator Mega1(9,8);

void setup() {
        Serial.begin(9600);
        Serial.println("Setup begin");
        Wire.begin();
        while (!Serial);
        Mega1.setup_pins();
        Mega1.search();
        attachInterrupt(digitalPinToInterrupt(2), count_first, RISING);
        attachInterrupt(digitalPinToInterrupt(3), count_second, RISING);
        Timer1.initialize(4*1000000);
        Timer1.attachInterrupt(keep_alive);
        Serial.println("Setup end");
}

void loop() {
        Mega1.address_range();
}

void count_first(){
        Mega1.setCounterOne(1);
}

void count_second(){
        Mega1.setCounterTwo(1);
}

void keep_alive(){
        if(Mega1.getCounterOne() >=1 && Mega1.getCounterOne() <=3) {
                Serial.print("keep alive 1st: ");
                Serial.println(Mega1.getCounterOne());
        }else{
                Serial.println("Board 1 is missing");
        }
        if(Mega1.getCounterTwo() >=1 && Mega1.getCounterTwo() <=3) {
                Serial.print("keep alive 2nd: ");
                Serial.println(Mega1.getCounterTwo());
        }else{
                Serial.println("Board 2 is missing");
        }

        Mega1.setCounterOne(0);
        Mega1.setCounterTwo(0);
}
