#include <Wire.h>
#include <TimerOne.h>
#include "/Users/nicoflorschutz/Documents/Bachelorthesis/eigeneBib/classMega.h"


Simulator Mega1(9, 8);

void setup() {
  
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Setup begin");
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
  Mega1.counter_first++;
  
}

void count_second(){
  Mega1.counter_second++;
}

void keep_alive(){
        if(Mega1.counter_first >=1 && Mega1.counter_first <=5) {
                Serial.print("keep alive 1st: ");
                Serial.println(Mega1.counter_first);
        }else{
                Serial.println("Board 1 is missing");
        }
        if(Mega1.counter_second >=1 && Mega1.counter_second <=5) {
                Serial.print("keep alive 2nd: ");
                Serial.println(Mega1.counter_second);
        }else{
                Serial.println("Board 2 is missing");
        }
        
        Mega1.counter_first = 0;
        Mega1.counter_second = 0;
}
