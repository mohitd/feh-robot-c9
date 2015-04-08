#include "cage.h"
#include <FEHUtility.h>
#include <FEHLCD.h>

#define MOTOR_SPEED 40

void Cage::Lower()
{
    LCD.WriteLine("Lowering cage...");
    if (cage_switch.Value())
    {
        // Raise the cage!
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
    LCD.WriteLine("Raising cage 1/2...");
    cageMotor.SetPercent(MOTOR_SPEED);
    while (crank_switch.Value());
    Sleep(500);
    while (crank_switch.Value());
    cageMotor.Stop();
}

void Cage::Lower3_4()
{
    LCD.WriteLine("Lowering cage 3/4...");
    cageMotor.SetPercent(-MOTOR_SPEED);
    Sleep(1500);
    cageMotor.Stop();
}

void Cage::Lower1_4()
{
    LCD.WriteLine("Lowering cage 1/4...");
    cageMotor.SetPercent(-MOTOR_SPEED);
    Sleep(1000);
    cageMotor.Stop();
}
