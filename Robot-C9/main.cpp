/**
 * @file main.cpp
 * The main code that runs the entire course
 */

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHBattery.h>
#include <cmath>

#include "drivetrain.h"
#include "cdscell.h"
#include "cage.h"
#include "diagnostics.h"

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

const int col_percent = 25;
void col_turn_clockwise()
{
    right_motor.SetPercent(col_percent);
    left_motor.SetPercent(col_percent);
    Sleep(800);
    col_motor.SetPercent(50);
    Sleep(3000);
    col_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

void col_turn_cclockwise()
{
    right_motor.SetPercent(col_percent);
    left_motor.SetPercent(col_percent);
    Sleep(800);
    col_motor.SetPercent(-50);
    Sleep(3000);
    col_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

void press_buttons()
{
    if (RPS.RedButtonOrder() == 1)
    {
        if (RPS.WhiteButtonOrder() == 2)
        {
            col_turn_clockwise();
        }
        else if (RPS.BlueButtonOrder() == 2)
        {
            col_turn_cclockwise();
        }
    }
    else if (RPS.WhiteButtonOrder() == 1)
    {
        col_motor.SetPercent(50);
        Sleep(250);
        col_motor.Stop();
        if (RPS.RedButtonOrder() == 2)
        {
            col_turn_clockwise();
        }
        else if (RPS.BlueButtonOrder() == 2)
        {
            col_turn_cclockwise();
        }
    }
    else if (RPS.BlueButtonOrder() == 1)
    {
        col_motor.SetPercent(-50);
        Sleep(250);
        col_motor.Stop();
        if (RPS.WhiteButtonOrder() == 2)
        {
            col_turn_clockwise();
        }
        else if (RPS.RedButtonOrder() == 2)
        {
            col_turn_cclockwise();
        }
    }
}

int main(void)
{
    ButtonBoard buttons( FEHIO::Bank3 );

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::Green );
    LCD.SetOrientation(FEHLCD::East);

    // Allocate mem for classes
    DriveTrain *driveTrain = new DriveTrain(left_motor, right_motor, left_encoder, right_encoder);
    CdSCell *cdsCell = new CdSCell(cds);
    Cage *cage = new Cage(cage_motor, cage_microswitch, crank_microswitch);
    Diagnostics *diagnostics = new Diagnostics(buttons, left_motor, right_motor, cage_motor, col_motor, left_encoder, right_encoder, cds, cage_microswitch, col_microswitch, crank_microswitch);

    LCD.WriteLine("Press middle button");
    while(!buttons.MiddlePressed()); //Wait for middle button to be pressed
    while(buttons.MiddlePressed()); //Wait for middle button to be unpressed

    LCD.WriteLine("Column Initialized?");
    LCD.WriteLine("Y(B) N(T)");
    while (!buttons.LeftPressed());
    while (buttons.LeftPressed());
    LCD.WriteLine("Good!");

//    diagnostics->RunDiagnostics();

    RPS.InitializeMenu();

    // Begin! Pick up the salt bag
    cdsCell->WaitForLight();
    driveTrain->Drive(FBDirection::Forward, 30, 10.0f);
    driveTrain->CheckHeading(0);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 5.0f);
    driveTrain->CheckHeading(0);
    driveTrain->CheckY(14, false);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 103);
    cage->Lower3_4();
    driveTrain->CheckHeading(47);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 8.0f);
    driveTrain->CheckHeading(47);
    driveTrain->Drive(FBDirection::Forward, 60, 10);
    cage->Lower1_4();

    driveTrain->Drive(FBDirection::Forward, 25, 5);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 8.0f);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 15.0f);
    driveTrain->CheckX(27.0f, false);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 80);

    // At the base of the ramp
    driveTrain->CheckHeading(0);
    driveTrain->CheckHeading(0);
    driveTrain->CheckY(23, false);
    Sleep(100);
    driveTrain->Drive(FBDirection::Backward, 65);
    while (RPS.Y() < 35);
    driveTrain->Stop();

    // At top of ramp
    driveTrain->CheckY(49, false);
    driveTrain->CheckHeading(0);
    bool adjusted = false;
    float coord = 0;
    if (RPS.X() < 29)
    {
        driveTrain->CheckHeading(285);
        adjusted = true;
        coord = 29.5;
    }
    else if (RPS.X() > 30)
    {
        driveTrain->CheckHeading(75);
        adjusted = true;
        coord = 30.5;
    }

    int start = TimeNowSec();
    while (abs(RPS.X() - coord) > .5 && TimeNowSec() - start < 10) driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 5);
    driveTrain->CheckHeading(0);

    // Rotate the crank
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 1.0f);
    driveTrain->CheckY(52.0f, false);
    cage->RaiseHalf();
    driveTrain->CheckY(56.5, false);
    driveTrain->Drive(FBDirection::Backward, 20);
    if (cdsCell->DetectColor() == Color::Blue)  cage_motor.SetPercent(-30);
    else cage_motor.SetPercent(30);
    Sleep(500);
    while (crank_microswitch.Value());
    Sleep(200);
    cage_motor.Stop();
    driveTrain->Stop();

    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 1.0f);
    cage->Raise();
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 2.0f);
    cage->Lower();
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 1.0f);
    driveTrain->CheckY(52.5f, false);

    // To the garage and deposit the salt bag
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->CheckHeading(270);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 12.0f);
    driveTrain->CheckX(10.5, false);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT, 113);
    driveTrain->CheckHeading(225);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 2.0f);
    cage->Raise();
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 3.0f);
    cage->Lower();
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 8.0f);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 7.0f);
    cage->Raise();

    // To the buttons
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 102);
    driveTrain->CheckHeading(270);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 3.0f);
    driveTrain->CheckX(23.3, false);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT, 102);
    driveTrain->CheckHeading(225);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 2.0f);
    driveTrain->CheckHeading(225);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 2.0f);
    driveTrain->CheckHeading(225);
    press_buttons();

    // Down ramp
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 14.0f);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT, 306);
    driveTrain->CheckHeading(90);
    driveTrain->CheckX(32, true);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->CheckHeading(0);
    driveTrain->Drive(FBDirection::Forward, 30, 20.0f);
    driveTrain->CheckY(15, false);

    // To oil pump
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->CheckHeading(270);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 4.0f);
    driveTrain->CheckX(14, false);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 102);
    driveTrain->CheckHeading(310);
    cage->Lower1_4();
    do
    {
        driveTrain->Drive(FBDirection::Forward, 50, 4.0f);
        driveTrain->Stop();
        driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 4.0f);
        driveTrain->CheckHeading(310);
    } while (RPS.OilPress() == 0);

    LCD.WriteLine("Done!");

    // Delete any allocated mem
    delete diagnostics;
    delete driveTrain;
    delete cdsCell;
    delete cage;
    return 0;
}

