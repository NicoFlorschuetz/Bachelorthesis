#include <simulation.h>
//#include "BoardClass.h"



FDIR_Master::FDIR_Master(){
        setup_pins();
}


Protocol::Protocol(){

}

bool Protocol::isMessageValid(){
        if(buffer[0] != 0) {
                return true;
        }else{
                return false;
        }
}

Message Protocol::receive(){
        Message new_message;
        new_message.id_board = buffer[0];
        new_message.failureCode = buffer[1];
        new_message.failure = buffer[2];
        new_message.value = buffer[3];
        return new_message;

}

void Protocol::readData(int id){
        Wire.requestFrom(id,7);
        int x = 0;
        while(x<=Wire.available()) {
                buffer[x] = Wire.read();
                x++;
        }
}

void Board::execute(void){
        const static struct Message resetMessage;

        protocol.readData(id);

        if (protocol.isMessageValid())
        {
                message = protocol.receive();
                Serial.print(message.id_board);
                Serial.print(message.failureCode);
                Serial.print(message.failure);
                Serial.println(message.value);
                handler.detect(id, message);
                message = resetMessage;
        }
}

void FailureHandler::detect(int id, Message message){
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
        Serial.print(id);
        Serial.print("\t");
        Serial.print(fehlerAnalyse.LevelNull);
        Serial.print(fehlerAnalyse.LevelOne);
        Serial.print(fehlerAnalyse.LevelTwo);
        Serial.print(fehlerAnalyse.LevelThree);
        Serial.println(fehlerAnalyse.LevelFour);


        /*if (antwort[0] != -1) {
                       if ( x >=3 ) {
                       }else if(antwort[1] == 1  && x>1) {
                               if (antwort[2] == 1) {
                                       Serial.println("Simple failure");
                                       Serial.print(antwort[1]);
                                       Serial.println(antwort[2]);
                               }else if (antwort[2]== 2) {
                                       Serial.println("failure is bigger");
                                       Serial.print(antwort[1]);
                                       Serial.println(antwort[2]);
                                       digitalWrite(RESET_ARDUINO1, LOW);
                                       delay(2000);
                                       digitalWrite(RESET_ARDUINO2, HIGH);
                               }
                       }else if(antwort[0] == -1) {
                               Serial.println("No connection, try to reconnect");
                               //address_range();
                       }
               }else{
                       Serial.print("Board ");
                       Serial.print(nodeAddress);
                       Serial.println(" ist down");
               }
               serial_input();
         */
}



bool FDIR_Master::reg(Executable *e){
        schedule[anzahlBoards]= e;
}

void FDIR_Master::searchForAddresses(){
        for(int search_address = 1; search_address < 120; search_address++) {
                Wire.beginTransmission(search_address);
                if(Wire.endTransmission() == 0) {
                        Board * const board = new Board(search_address);
                        boards[anzahlBoards] = board;
                        reg (dynamic_cast<Executable*>(board));
                        /*Wire.beginTransmission(search_address);
                           Wire.write(1);
                           Wire.endTransmission();
                           delay(50);
                           if(counter_first == 1) {
                                BoardIdToInterruptPin[0][0] = PIN_FIRST;
                                BoardIdToInterruptPin[0][1] = search_address;


                           }else if(counter_second == 1) {
                                BoardIdToInterruptPin[1][0] = PIN_SECOND;
                                BoardIdToInterruptPin[1][1] = search_address;
                           }*/
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
        digitalWrite(RESET_ARDUINO1, HIGH);
        digitalWrite(RESET_ARDUINO2, HIGH);
        pinMode(RESET_ARDUINO1, OUTPUT);
        pinMode(RESET_ARDUINO2, OUTPUT);
        pinMode(PIN_FIRST, INPUT);
        pinMode(PIN_SECOND, INPUT);
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



/*class Protocol {
   private:
   int buffer[7];
   int id;
   public:
   bool isMessageValid(void) {
        if(new_message.id_board != 0 && new_message.length == 42) {
                return true;
        }

   }

   Protocol()
   {
   }

   Message receive()
   {

        // Message <- buffer
        // clear buffer/message
        //baue empfangene werte zu message zusammen
        //std::cout << "Receive protocol\n";

        new_message.length += 4;
        new_message.id_board = buffer[0];
        new_message.failure = buffer[1];
        new_message.failureCode = buffer[2];
        new_message.keep_alive_HP = buffer[3];

        const static struct Message resetMessage;
        new_message = resetMessage;
   }

   void readData(int id)
   //void execute(void)  //
   {
        Wire.requestFrom(id,7);
        int x = 0;
        while(x<=Wire.available()) {
                buffer[x] = Wire.read();
                x++;
        }
        // receive single bytes over wire
   }
   };*/

/*class Board : public Executable {
   int id;
   FailureHandler handler;
   Message message;
   Protocol protocol;

   public:
   Board() : id(0) {   //was macht ":" und was passiert hier
   }

   Board(int id)
        : protocol()
   {
        this->id = id;
   }

   void execute(void)
   {
        // assert(id != 0);
        protocol.readData(id);

        if (protocol.isMessageValid())
        {
                message = protocol.receive();
                handler.detect(id, message);
        }
   }
   };*/

/*class FDIR_Master
   {
   private:
   std::list<Board*> boards;
   std::list<Executable*> schedule;

   bool reg (Executable * e)
   {
        schedule.push_back(e);
   }

   enum { forever = 1 };

   public:
   FDIR_Master()
   {
        // init
        //search for boards and add them to Executable
        //wird hier im constructor aufgerufen und erstellt boards obwohl FDIR_Master noch nicht vollständig aufgebaut
        for(int search_address = 1; search_address < 120; search_address++) {
                Wire.beginTransmission(search_address);
                if(Wire.endTransmission() == 0) {
                        //Board board(search_address);
                        Board * const board = new Board(search_address);
                        boards.push_back(board);
                        reg (dynamic_cast<Executable*>(board));
                }
                delay(5);
        }

        do
        {
                for(auto s : schedule)
                {
                        s->execute();
                }
        }
        while(forever);

   }


   };*/
