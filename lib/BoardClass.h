#ifndef BoardClass_h
#define BoardClass_h




class Board {
private:
int _received_Data[4];
int _Address;
int ID;
Simulator *m_sim;

public:

Board(int Address, int received_Data[4]);

void create_ID();


};


#endif
