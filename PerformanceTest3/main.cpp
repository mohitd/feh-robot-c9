#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

#include "drivetrain.h"
#include "logger.h"
#include "cage.h"

#define TIMEOUT 5
#define MOTOR_PERCENT_CHECKS 25
#define MOTOR_PERCENT 40
#define CDS_THRESHOLD 2

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P0_1);
AnalogInputPin cds(FEHIO::P0_2);
DigitalInputPin microswitch(FEHIO::P0_3);

DigitalInputPin cageSwitch(FEHIO::P1_1);

FEHMotor left_motor(FEHMotor::Motor0);
FEHMotor right_motor(FEHMotor::Motor1);
FEHMotor vex_motor(FEHMotor::Motor2);
FEHMotor cage_motor(FEHMotor::Motor3);

void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
    LCD.Write("Checking x coord: ");
    LCD.WriteLine(x_coordinate);

    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        if(RPS.X() > x_coordinate)
        {
            LCD.WriteLine("Too far!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.X() < x_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }

    Sleep(500);
}

void check_x_minus(float x_coordinate) //using RPS while robot is in the +x direction
{
    LCD.Write("Checking x coord: ");
    LCD.WriteLine(x_coordinate);

    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        if(RPS.X() > x_coordinate)
        {
            LCD.WriteLine("Too far!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.X() < x_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }

    Sleep(500);
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    LCD.Write("Checking y coord: ");
    LCD.WriteLine(y_coordinate);

    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if(RPS.Y() > y_coordinate)
        {
            LCD.WriteLine("Too far!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.Y() < y_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }

    Sleep(500);
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    LCD.Write("Checking y coord: ");
    LCD.WriteLine(y_coordinate);

    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if(RPS.Y() > y_coordinate)
        {
            LCD.WriteLine("Too far!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(-MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.Y() < y_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            left_motor.SetPercent(MOTOR_PERCENT_CHECKS);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }

    Sleep(500);
}

void check_heading(DriveTrain& driveTrain, float heading) //using RPS
{
    LCD.Write("Checking heading: ");
    LCD.WriteLine(heading);

    double start = TimeNow();
    if (heading == 0)
    {
        while ((RPS.Heading() > 1 || RPS.Heading() < 359) && TimeNow() - start < TIMEOUT)
        {
            if (RPS.Heading() >= 180)
            {
                driveTrain.Turn(LRDirection::Left, 20, 3);
            }
            else
            {
                driveTrain.Turn(LRDirection::Right, 20, 3);
            }
        }
    }

    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)
    while (RPS.Heading() < heading - 1 || RPS.Heading() > heading + 1 || TimeNowSec() - start > TIMEOUT)
    {
        if (RPS.Heading() > heading)
        {
            LCD.WriteLine("Too counterclockwise!");
            driveTrain.Turn(LRDirection::Right, MOTOR_PERCENT_CHECKS, 3);
        }
        else if (RPS.Heading() < heading)
        {
            LCD.WriteLine("Too clockwise!");
            driveTrain.Turn(LRDirection::Left, MOTOR_PERCENT_CHECKS, 3);
        }
    }
}

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

//    LCD.WriteLine("Waiting for light...");
//    while (cds.Value() > 0.17);

    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 6.0f);
    check_y_minus(14);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 40);
    check_heading(*driveTrain, 45);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 7.5f);
    check_heading(*driveTrain, 45);
    cage->Lower();
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 8.0f);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 14.0f);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 80);
    check_heading(*driveTrain, 0);
    check_y_minus(24.5);
    driveTrain->Drive(FBDirection::Backward, 60, 18.0f);
    check_y_minus(42.5);
//    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
//    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 8.0f);
//    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT, 40);
//    check_heading(*driveTrain, 0);
//    check_y_minus(24.5);
//    driveTrain->Drive(FBDirection::Backward, 60, 12.0f);
//    check_y_minus(42.5);
//    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 4.0f);
//    check_y_minus(56);
//    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
//    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 12.0f);
//    check_x_minus(6);
//    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT, 20);
//    check_heading(*driveTrain, 225);

    LCD.WriteLine("Done!!!");

    delete driveTrain;
    return 0;
}

