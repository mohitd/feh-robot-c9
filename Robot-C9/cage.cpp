#include "cage.h"
#include <FEHUtility.h>
#include <FEHLCD.h>

#define MOTOR_SPEED 40

void Cage::Lower()
{
    LCD.WriteLine("Lowering cage...");
    if (cage_switch.Value())
    {
        Raise();
        Sleep(100);
    }
    cageMotor.SetPercent(-MOTOR_SPEED);
    Sleep(2750);
    cageMotor.Stop();
}

void Cage::Raise()
{
    LCD.WriteLine("Raising cage...");
    cageMotor.SetPercent(MOTOR_SPEED);
    while (cage_switch.Value());
    cageMotor.Stop();
}

void Cage::RaiseHalf()
{
    LCD.WriteLine("Raising cage halfway...");
    cageMotor.SetPercent(MOTOR_SPEED);
    while (crank_switch.Value());
    Sleep(500);
    while (crank_switch.Value());
    cageMotor.Stop();
}