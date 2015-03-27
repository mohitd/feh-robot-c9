#include "cage.h"
#include <FEHUtility.h>

void Cage::Lower()
{
    if (microswitch.Value())
    {
        Raise();
        Sleep(500);
    }
    cageMotor.SetPercent(-50);
    Sleep(1500);
    cageMotor.Stop();
}

void Cage::Raise()
{
    cageMotor.SetPercent(50);
    while (microswitch.Value());
    cageMotor.Stop();
}

void Cage::RaiseHalf()
{
    cageMotor.SetPercent(50);
    Sleep(500);
    cageMotor.Stop();
}
