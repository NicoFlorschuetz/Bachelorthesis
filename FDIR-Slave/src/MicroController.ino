#include "Wire.h"
#include "classMicro.h"
#include "TimerOne.h"

#define TARGET2
#ifdef TARGET1
//Id aufbau: ID[0] = Arduino id; ID[1]= fehler oder nicht ; ID[2] = fehler status
int ID[] = {0,11, 0, 40};
//The constructor of the class FDIR_Slave consists of GREEN_LED, RED_LED, ANALOG_PIN, ID, I2C_ADDRESS
FDIR_Slave Arduino1(10, 8,  12, ID, 8);
#endif

#ifdef TARGET2
int ID[] = {0, 10, 0, 30};
FDIR_Slave Arduino1(10, 8,  12, ID, 9);
#endif


void setup() {
        Arduino1.board_setup();
        attachInterrupt(digitalPinToInterrupt(3), counterFirst, RISING);
        //call keep_alive funciton at interrupt
        Timer1.initialize(4*1000000);
        Timer1.attachInterrupt(keep_alive);
        Wire.onRequest(requestEvent);

}

//send keep alive over pin 4
void keep_alive(){
        if(Arduino1.getCounter() >=1 && Arduino1.getCounter() <=3) {
                Arduino1.boardIsOkay = true;
        }else{
                Arduino1.boardIsOkay = false;
        }
        Arduino1.setCounter(0);
}

void counterFirst(int pin){
        Arduino1.setCounter(1);
}



void loop() {
        //wait on signal at pin 2
        if(digitalRead(2) == HIGH) {
                digitalWrite(10, HIGH);
                digitalWrite(10, LOW);
        }
        Arduino1.sensor_reading();
}



void requestEvent()
{
        //at first time (ID[0] == 0) ID is a setup ID with reset pin, Reset mode and output pin
        if (Arduino1.ID[0] != 0) {

                Arduino1.ID[1]= Arduino1.getFailurecode();
                Arduino1.ID[2] = Arduino1.problemStatus.Normal;
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
        }else if (Arduino1.ID[0] == 0) {
                for (int x = 0; x < sizeof(Arduino1.ID)/sizeof(Arduino1.ID[0]); x++) {
                        Wire.write(Arduino1.ID[x]);
                }
                #ifdef TARGET1
                Arduino1.ID[0] = 1;
                #endif
                #ifdef TARGET2
                Arduino1.ID[0] = 2;
                #endif
        }


}
