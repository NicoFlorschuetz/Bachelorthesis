#include <TimerOne.h>
#include <simulation.h>

FDIR_Master Mega;

void setup() {
        Serial.begin(9600);
        Serial.println("Setup begin");
        Wire.begin();
        while (!Serial);
        Mega.setup_pins();
        Mega.searchForAddresses();
        //Mega.setup_pins();
        //Mega1.search();
        attachInterrupt(digitalPinToInterrupt(PIN_FIRST), count_first, RISING);
        attachInterrupt(digitalPinToInterrupt(PIN_SECOND), count_second, RISING);
        Timer1.initialize(4*1000000);
        Timer1.attachInterrupt(keep_alive);
        Serial.println("Setup end");
}

void loop() {
        Mega.doScheduling();
        delay(100);
}

void count_first(int pin){
        Mega.setCounterOne(1);
}



void count_second(){
        Mega.setCounterTwo(1);
}

void keep_alive(){

        if(Mega.getCounterOne() >=1 && Mega.getCounterOne() <=3) {
                Serial.print("keep alive 1st: ");
                Serial.println(Mega.getCounterOne());
        }else{
                Serial.println("Board 1 is missing");
        }
        if(Mega.getCounterTwo() >=1 && Mega.getCounterTwo() <=3) {
                Serial.print("keep alive 2nd: ");
                Serial.println(Mega.getCounterTwo());
        }else{
                Serial.println("Board 2 is missing");
        }

        Mega.setCounterOne(0);
        Mega.setCounterTwo(0);
}
