#include "Wire.h"
#include "classMicro.h"
#include "TimerOne.h"

volatile unsigned int counter= 0;
int command = 0;

#define TARGET2
#ifdef TARGET1
//Id aufbau: ID[0] = Arduino id; ID[1]= fehler oder nicht ; ID[2] = fehler status
int ID[] = {0,2, 0, 40};
//The constructor of the class FDIR_Slave consists of GREEN_LED, RED_LED, ANALOG_PIN, ID, I2C_ADDRESS
FDIR_Slave Arduino1(10, 8,  12, ID, 8);
#endif

#ifdef TARGET2
int ID[] = {0, 3, 0, 30};
FDIR_Slave Arduino1(10, 8,  12, ID, 9);
#endif
void setup() {
        Arduino1.board_setup();
        Timer1.initialize(4*1000000);
        Timer1.attachInterrupt(keep_alive);
        //Wire.onReceive(ReceiveCommandFromMaster);
        Wire.onRequest(requestEvent);

}

/*void ReceiveCommandFromMaster(int anzahl){
        int receive;
        while(0<Wire.available()) {
                receive = Wire.read();
                if(receive == 1) {//receive[0] == 0 && receive[1]==1) {
                        digitalWrite(10, HIGH);
                        digitalWrite(10, LOW);

                }
        }
   }*/

void keep_alive(){
        digitalWrite(4,HIGH);
        digitalWrite(4,LOW);
}

void loop() {

        if(digitalRead(2) == HIGH) {
                digitalWrite(10, HIGH);
                digitalWrite(10, LOW);
        }
        Arduino1.sensor_reading();
}



void requestEvent()
{
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
                Serial.print(Arduino1.ID[0]);
                Serial.print(Arduino1.ID[1]);
                Serial.print(Arduino1.ID[2]);
                Serial.println(Arduino1.ID[3]);
                /*for (int x = 0; x < sizeof(Arduino1.ID)/sizeof(Arduino1.ID[0]); x++) {
                     Wire.write(Arduino1.ID[x]);
                   }*/
                for (int x = 0; x < sizeof(Arduino1.ID)/sizeof(Arduino1.ID[0]); x++) {
                        Wire.write(Arduino1.ID[x]);
                }
        }else if (Arduino1.ID[0] == 0) {

                Serial.print(Arduino1.ID[0]);
                Serial.print(Arduino1.ID[1]);
                Serial.print(Arduino1.ID[2]);
                Serial.println(Arduino1.ID[3]);
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
