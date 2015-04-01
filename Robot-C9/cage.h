#ifndef CAGE_H
#define CAGE_H

#include <FEHMotor.h>
#include <FEHIO.h>

class Cage
{
public:
    Cage(FEHMotor& cageMotor, DigitalInputPin& cage_switch, DigitalInputPin& crank_switch) : cageMotor(cageMotor), cage_switch(cage_switch), crank_switch(crank_switch)
    {

    }

    void Lower();
    void Raise();
    void RaiseHalf();

private:
    FEHMotor& cageMotor;
    DigitalInputPin& cage_switch;
    DigitalInputPin& crank_switch;
};

#endif // CAGE_H
