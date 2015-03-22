#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

#include "drivetrain.h"
#include "logger.h"
#include "cage.h"

#define TIMEOUT 5
#define MOTOR_PERCENT 40
#define CDS_THRESHOLD 2.9

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P0_1);
AnalogInputPin cds(FEHIO::P2_7);
DigitalInputPin microswitch(FEHIO::P0_3);

DigitalInputPin cageSwitch(FEHIO::P1_1);

FEHMotor left_motor(FEHMotor::Motor0);
FEHMotor right_motor(FEHMotor::Motor1);
FEHMotor vex_motor(FEHMotor::Motor2);
FEHMotor cage_motor(FEHMotor::Motor3);

void col_turn_clockwise()
{
    right_motor.SetPercent(27);
    left_motor.SetPercent(27);
    Sleep(800);
    vex_motor.SetPercent(50);
    Sleep(3000);
    vex_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

void col_turn_cclockwise()
{
    right_motor.SetPercent(27);
    left_motor.SetPercent(27);
    Sleep(800);
    vex_motor.SetPercent(-50);
    Sleep(3000);
    vex_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

void press_buttons(FEHMotor& vex)
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
        vex.SetPercent(50);
        Sleep(250);
        vex.Stop();
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
        vex.SetPercent(-50);
        Sleep(250);
        vex.Stop();
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
    LCD.Clear(FEHLCD::Black);
    LCD.SetFontColor(FEHLCD::White);

    DriveTrain *driveTrain = new DriveTrain(left_motor, right_motor, left_encoder, right_encoder);
    Cage *cage = new Cage(cage_motor, cageSwitch);

    LCD.WriteLine("Press middle button");
    while(!buttons.MiddlePressed()); //Wait for middle button to be pressed
    while(buttons.MiddlePressed()); //Wait for middle button to be unpressed

    LCD.WriteLine("Did you initialize the column? Y(R) N(L)");
    while (!buttons.RightPressed());
    while (buttons.RightPressed());
    LCD.WriteLine("Good!");

    RPS.InitializeMenu();

    LCD.WriteLine("Waiting for light...");
    while (cds.Value() > CDS_THRESHOLD)
    {
        LCD.Write("CdS Value: ");
        LCD.WriteLine(cds.Value());
        Sleep(1000);
    }

    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 7.5f);
    check_y_minus(14);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 40);
    check_heading(*driveTrain, 47);
    Sleep(3.0);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 9.0f);
    check_heading(*driveTrain, 47);
    cage->Lower();
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT_CHECKS, 5);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 8.0f);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 13.5f);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 80);
    check_heading(*driveTrain, 0);
    check_y_minus(25);
    driveTrain->Drive(FBDirection::Backward, 70, 35.0f);
    check_y_minus(50);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    check_heading(*driveTrain, 270);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 18.0f);
    check_heading(*driveTrain, 270);
    check_x_minus(12.5);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT, 50);
    check_heading(*driveTrain, 225);
    cage->Raise();
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 2.5f);
    cage->Lower();
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 8.0f);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 6.0f);
    cage->Raise();

    LCD.WriteLine("Done!!!");

    delete driveTrain;
    delete cage;
    return 0;
}

