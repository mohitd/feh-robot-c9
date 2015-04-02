/**
 * @file main.cpp
 * The main code that runs the entire course
 */

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <cmath>

#include "drivetrain.h"
#include "cdscell.h"
#include "cage.h"

#define MOTOR_PERCENT 40

using namespace std;

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P0_1);
AnalogInputPin cds(FEHIO::P1_0);
DigitalInputPin cage_microswitch(FEHIO::P2_0);
DigitalInputPin col_microswitch(FEHIO::P2_1);
DigitalInputPin crank_microswitch(FEHIO::P2_2);

FEHMotor left_motor(FEHMotor::Motor0);
FEHMotor right_motor(FEHMotor::Motor1);
FEHMotor col_motor(FEHMotor::Motor2);
FEHMotor cage_motor(FEHMotor::Motor3);

int main(void)
{
    ButtonBoard buttons( FEHIO::Bank3 );

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    // Allocate mem for classes
    DriveTrain *driveTrain = new DriveTrain(left_motor, right_motor, left_encoder, right_encoder);
    CdSCell *cdsCell = new CdSCell(cds);
    Cage *cage = new Cage(cage_motor, cage_microswitch, crank_microswitch);

    LCD.WriteLine("Press middle button");
    while(!buttons.MiddlePressed()); //Wait for middle button to be pressed
    while(buttons.MiddlePressed()); //Wait for middle button to be unpressed

    LCD.WriteLine("Column Initialized? Y(R) N(L)");
    while (!buttons.RightPressed());
    while (buttons.RightPressed());
    LCD.WriteLine("Good!");

    RPS.InitializeMenu();

    // Begin! Pick up the salt bag
    cdsCell->WaitForLight();
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 16.0f);
    driveTrain->CheckY(14, false);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 103);
    driveTrain->CheckHeading(47);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 8.0f);
    driveTrain->CheckHeading(47);
    cage->Lower();
    driveTrain->Drive(FBDirection::Forward, 25, 5);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 8.0f);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 14.0f);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 80);

    // At the base of the ramp
    driveTrain->CheckHeading(0);
    driveTrain->CheckHeading(0);
    driveTrain->CheckY(23, false);
    Sleep(200);
    driveTrain->Accelerate(FBDirection::Backward, MOTOR_PERCENT, 65, 2.90f);
    driveTrain->CheckY(50, false);
    driveTrain->CheckHeading(0);
    if (RPS.X() <= 30) driveTrain->CheckHeading(340);
    else if (RPS.X() >= 32) driveTrain->CheckHeading(20);
    while (abs(RPS.X() - 31) > 1) driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 5);
    driveTrain->CheckHeading(0);

    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 4.0f);

    if (RPS.X() <= 30) driveTrain->CheckHeading(340);
    else if (RPS.X() >= 32) driveTrain->CheckHeading(20);
    while (abs(RPS.X() - 31) > 1) driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 5);
    driveTrain->CheckHeading(0);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 3.0f);

    // Rotate the crank
    cage->RaiseHalf();
    driveTrain->CheckY(56.5, false);
    if (cdsCell->DetectColor() == Color::Blue)  cage_motor.SetPercent(-30);
    else cage_motor.SetPercent(30);
    Sleep(500);
    while (crank_microswitch.Value());
    Sleep(200);
    cage_motor.Stop();    
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 1.0f);
    cage->Raise();
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 2.0f);
    cage->Lower();

    // To the garage and deposit the salt bag
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->CheckHeading(270);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 16.0f);
    driveTrain->CheckHeading(270);
    driveTrain->CheckX(12.5, false);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT, 50);
    driveTrain->CheckHeading(225);
    cage->Raise();
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 2.5f);
    cage->Lower();
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 8.0f);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 6.0f);
    cage->Raise();

    // To the buttons
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 2.0f);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT);
    driveTrain->CheckHeading(225);

    LCD.WriteLine("Done!");

    // Delete any allocated mem
    delete driveTrain;
    delete cdsCell;
    delete cage;
    return 0;
}

