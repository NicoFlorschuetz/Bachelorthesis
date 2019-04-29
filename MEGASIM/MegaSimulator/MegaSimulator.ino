#include <Wire.h>
#include "/Users/nicoflorschutz/Documents/Bachelorthesis/eigeneBib/classMega.h"



Simulator Mega1(8, 9, false, false);

void setup() {
  // put your setup code here, to run once:
  Mega1.setup();
}

void loop() {

  // put your main code here, to run repeatedly:
  Mega1.loop();
}
