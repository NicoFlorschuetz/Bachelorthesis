#include <simulation.h>

FDIR_Master::FDIR_Master(){
        setup_pins();
}


Protocol::Protocol(){

}

bool Protocol::isMessageValid(){
        return (buffer[0] != 0);
}

//create new message
static Message Protocol::receive(){
        Message new_message;
        new_message.id_board = buffer[0];
        new_message.failureCode = buffer[1];
        new_message.failureSolved = buffer[2];
        new_message.value = buffer[3];
        return new_message;

}

//create new setup Message
static Setup_Message Protocol::receiveSetup(){
        Setup_Message new_setup_Message;
        new_setup_Message.keepAlivePin = buffer[1];
        new_setup_Message.fdirActions = buffer[2];
        new_setup_Message.fdirActionParam = buffer[3];
        return new_setup_Message;
}

//get data via I2C
void Protocol::readData(int id){
        Wire.requestFrom(id,7);
        int x = 0;
        while(x<=Wire.available()) {
                buffer[x] = Wire.read();
                x++;
        }
}

//send message to slave via I2C
void Protocol::sendMessage(int id, MessageToSlave msg){
        Wire.beginTransmission(id);
        Wire.write(msg);
        Wire.endTransmission();
}


//main function
void Board::execute(void){
        if(healthstatus.health == 1) {
                const static struct Message resetMessage;
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
                                        healthstatus.health = false;
                                        //fdirActions->execute();
                                }
                        }
                        message = resetMessage;
                }else if(!protocol.isMessageValid()) {
                        setup_Message = protocol.receiveSetup();
                        Serial.print(setup_Message.keepAlivePin);
                        Serial.print(setup_Message.fdirActions);
                        Serial.println(setup_Message.fdirActionParam);
                        this->fdirActions = FdirActionFactory::getFdirAction(setup_Message.fdirActions, setup_Message.keepAlivePin);
                }
        }
}


//analyse failure with the help of the received message
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

//set the reset mode
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

//shut down collapsed board
void PinReset::execute(void){
        digitalWrite(pin, LOW);
}

void PinReset::reset(void){

        //not used
}

//send message to slave
void WireReset::execute(){
        if(messageSent == false) {
                protocol.sendMessage(this->id, RETURN_MSG);
                messageSent = true;
        }
}

//set messageSent to false, so Master can send message just for one time
void WireReset::reset(){
        messageSent = false;
}

//add new Board to schedule
bool FDIR_Master::reg(Executable *e){
        schedule[anzahlBoards]= e;
}

//iterates over 127 possible addresses and wait for request from slave
//and create new board object for each address
void FDIR_Master::searchForAddresses(){
        delay(1000);
        for(int search_address = 1; search_address < 127; search_address++) {
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

//for each board in scheduling
void FDIR_Master::doScheduling(){
        for(int i = 0; i< anzahlBoards; i++)
        {
                schedule[i]->execute();
        }
}

//first setup for pins
void FDIR_Master::setup_pins(){
        digitalWrite(40, LOW);
        digitalWrite(30, LOW);
        pinMode(40, OUTPUT);
        pinMode(30, OUTPUT);
        digitalWrite(40, LOW);
        digitalWrite(30, LOW);

}
