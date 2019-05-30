#include <simulation.h>
//#include "BoardClass.h"


FDIR_Master::FDIR_Master(){
        setup_pins();
}


Protocol::Protocol(){

}

bool Protocol::isMessageValid(){
        return (buffer[0] != 0);
}

static Message Protocol::receive(){
        Message new_message;
        new_message.id_board = buffer[0];
        new_message.failureCode = buffer[1];
        new_message.failureSolved = buffer[2];
        new_message.value = buffer[3];
        return new_message;

}

static Setup_Message Protocol::receiveSetup(){
        Setup_Message new_setup_Message;
        new_setup_Message.keepAlivePin = buffer[1];
        new_setup_Message.fdirActions = buffer[2];
        new_setup_Message.fdirActionParam = buffer[3];
        return new_setup_Message;
}

void Protocol::readData(int id){
        Wire.requestFrom(id,7);
        delay(5);
        int x = 0;
        while(x<=Wire.available()) {
                buffer[x] = Wire.read();
                x++;

        }
}

void Protocol::sendMessage(int id, MessageToSlave msg){
        Wire.beginTransmission(id);
        Wire.write(msg);
        Wire.endTransmission();
}



void Board::execute(void){
        if(health) {
                const static struct Message resetMessage;
                const static struct Setup_Message setupReset;

                protocol.readData(id);

                if (protocol.isMessageValid())
                {

                        message = protocol.receive();
                        Serial.print(message.id_board);
                        Serial.print(message.failureCode);
                        Serial.print(message.failureSolved);
                        Serial.println(message.value);
                        failureLevel = handler.detect(id, message);
                        if(fdirActions != NULL) {
                                if(failureLevel.LevelNull) {
                                        fdirActions->reset();
                                }else if (failureLevel.LevelThree) {
                                        fdirActions->execute();
                                }
                        }
                        message = resetMessage;
                }   else if(!protocol.isMessageValid()) {
                        setup_Message = protocol.receiveSetup();
                        Serial.print(setup_Message.keepAlivePin);
                        Serial.print(setup_Message.fdirActions);
                        Serial.println(setup_Message.fdirActionParam);
                        this->fdirActions = FdirActionFactory::getFdirAction(setup_Message.fdirActions, setup_Message.fdirActionParam);
                }
        }


}




FailureAnalysis FailureHandler::detect(int id, Message message){
        FailureAnalysis fehlerAnalyse;
        switch (message.failureCode) {
        case 0:
                fehlerAnalyse.LevelNull = true;
                break;
        case 1:
                fehlerAnalyse.LevelOne = true;
                break;
        case 2:
                fehlerAnalyse.LevelTwo = true;
                break;
        case 3:
                fehlerAnalyse.LevelThree = true;

                break;
        case 4:
                fehlerAnalyse.LevelFour = true;
                break;
        }
        return fehlerAnalyse;
}

FdirAction* FdirActionFactory::getFdirAction(FDIR_ACTION_t fdirActions, int param){
        FdirAction *ret;
        switch(fdirActions)
        {
        case STANDARD_FDIR:
                ret = new PinReset(param); //--> Pin
                break;
        case WIRE_FDIR:
                ret = new WireReset(param); // --> id
                break;
        default:
                break;
        }

        return ret;
}

void PinReset::execute(){
        digitalWrite(pin, HIGH);
        digitalWrite(pin, LOW);
}

void PinReset::reset(){

        //not used
}

void WireReset::execute(){
        if(messageSent == false) {
                protocol.sendMessage(this->id, 1);
                messageSent = true;
        }
}

void WireReset::reset(){
        messageSent = false;
}

bool FDIR_Master::reg(Executable *e){
        schedule[anzahlBoards]= e;
}

void FDIR_Master::searchForAddresses(){
        delay(1000);
        for(int search_address = 1; search_address < 120; search_address++) {
                Wire.beginTransmission(search_address);
                if(Wire.endTransmission() == 0) {
                        Board * const board = new Board(search_address);
                        boards[anzahlBoards] = board;
                        reg (dynamic_cast<Executable*>(board));
                        anzahlBoards++;
                }
                delay(5);
        }
}

void FDIR_Master::doScheduling(){
        for(int i = 0; i< anzahlBoards; i++)
        {
                schedule[i]->execute();
        }
}

void FDIR_Master::setup_pins(){
        digitalWrite(40, LOW);
        digitalWrite(30, LOW);
        digitalWrite(RESET_ARDUINO1, HIGH);
        digitalWrite(RESET_ARDUINO2, HIGH);
        pinMode(RESET_ARDUINO1, OUTPUT);
        pinMode(RESET_ARDUINO2, OUTPUT);
        pinMode(PIN_FIRST, INPUT);
        pinMode(PIN_SECOND, INPUT);
        pinMode(40, OUTPUT);
        pinMode(30, OUTPUT);
        digitalWrite(40, LOW);
        digitalWrite(30, LOW);

}

void FDIR_Master::setCounterOne(int i){
        if (i == 1) {
                counter_first += i;
        }else if(i == 0) {
                counter_first = 0;
        }
}

void FDIR_Master::setCounterTwo(int i){
        if(i == 1) {
                counter_second += i;
        }else if(i == 0) {
                counter_second = 0;
        }
}

int FDIR_Master::getCounterOne(){
        return counter_first;
}

int FDIR_Master::getCounterTwo(){
        return counter_second;
}
