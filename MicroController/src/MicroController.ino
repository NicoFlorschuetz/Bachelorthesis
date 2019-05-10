#include "Wire.h"
#include <classMicro.h>

//Id aufbau: ID[0] = Arduino id; ID[1]= fehler oder nicht ; ID[2] = fehler status
int ID[] = {1, 0, 0, 0};
volatile unsigned int counter= 0;
int command;


//The constructor of the class Pins consists of GREEN_LED, RED_LED, ANALOG_PIN, ID, I2C_ADDRESS
Pins Arduino1(10, 8,  12, ID, 8);

void setup() {
        //Wire.onReceive(receiveEvent);
        Arduino1.board_setup();
        Wire.onRequest(requestEvent);
}

void loop() {
        digitalWrite(4, LOW);
        delayMicroseconds(10);
        counter++;
        if(counter == 100) {
                digitalWrite(4,HIGH);
                counter = 0;
        }
        //Arduino1.send_keep_alive_signal(counter);
        //counter++;
        Arduino1.sensor_reading();
}



void requestEvent()
{
        if (Arduino1.getFehlermeldung() == SIMPLE_FAILURE) {
                Arduino1.ID[1] = 1;
                Arduino1.ID[2] = Arduino1.getFehlermeldung();
                for (int x = 0; x < Arduino1.ID; x++) {
                        Wire.write(Arduino1.ID[x]);
                }
        }else if (Arduino1.getFehlermeldung() == 02) {
                Arduino1.ID[1] = 1;
                Arduino1.ID[2] =Arduino1.getFehlermeldung();
                for (int x = 0; x < Arduino1.ID; x++) {
                        Wire.write(Arduino1.ID[x]);
                }
        }else{
                Arduino1.ID[1] = 0;
                Arduino1.ID[2] = 0;
                for (int x = 0; x <= sizeof(Arduino1.ID)/sizeof(Arduino1.ID[0]); x++) {
                        if (x == 3) {
                                Arduino1.ID[x] = realValue;
                        }
                        Wire.write(Arduino1.ID[x]);
                }
        }
}



void receiveEvent(int bytes) {
        command = Wire.read();
}
