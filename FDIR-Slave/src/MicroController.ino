#include "Wire.h"
#include "classMicro.h"
#include "TimerOne.h"

volatile unsigned int counter= 0;
int command = 0;

#define TARGET1
#ifdef TARGET1
//Id aufbau: ID[0] = Arduino id; ID[1]= fehler oder nicht ; ID[2] = fehler status
int ID[] = {1, 0, 0, 0};
//The constructor of the class FDIR_Slave consists of GREEN_LED, RED_LED, ANALOG_PIN, ID, I2C_ADDRESS
FDIR_Slave Arduino1(10, 8,  12, ID, 8);
#endif

#ifdef TARGET2
int ID[] = {2, 0, 0, 0};
FDIR_Slave Arduino1(10, 8,  12, ID, 9);
#endif

void setup() {
        //Wire.onReceive(keep_alive);
        Arduino1.board_setup();
        Timer1.initialize(4*1000000);
        Timer1.attachInterrupt(keep_alive);
        Wire.onRequest(requestEvent);

}

void keep_alive(){
        //  int x = Wire.read();
        //  if(x == '1') {
        digitalWrite(4,HIGH);

        digitalWrite(4,LOW);
        //  }

}

void loop() {
        Arduino1.sensor_reading();
}

void requestEvent()
{
        Arduino1.ID[1]= Arduino1.getFailurecode();
        Arduino1.ID[2] = Arduino1.solveProblemSuccess.Solved;
        Arduino1.ID[3] = realValue;
        if(Arduino1.getFailurecode() > 4) {
                Arduino1.ID[0] = 0;
                Arduino1.ID[1] = 0;
                Arduino1.ID[2] = 0;
                Arduino1.ID[3] = 0;
        }
        for (int x = 0; x < sizeof(Arduino1.ID)/sizeof(Arduino1.ID[0]); x++) {
                Wire.write(Arduino1.ID[x]);
        }
}
