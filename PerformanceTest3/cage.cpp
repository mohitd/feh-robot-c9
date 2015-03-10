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
    Sleep(1300);
    cageMotor.Stop();
}

void Cage::Raise()
{
    cageMotor.SetPercent(50);
    while (microswitch.Value());
    cageMotor.Stop();
}
