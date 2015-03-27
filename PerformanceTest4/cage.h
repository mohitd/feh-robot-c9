#ifndef CAGE_H
#define CAGE_H

#include <FEHMotor.h>
#include <FEHIO.h>

class Cage
{
public:
    Cage(FEHMotor& cageMotor, DigitalInputPin& microswitch) : cageMotor(cageMotor), microswitch(microswitch)
    {

    }

    void Lower();
    void Raise();
    void RaiseHalf();

private:
    FEHMotor& cageMotor;
    DigitalInputPin& microswitch;
};

#endif // CAGE_H
