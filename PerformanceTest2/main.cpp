#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

#include "drivetrain.h"
#include "logger.h"

#define MOTOR_PERCENT 40
#define CDS_THRESHOLD 2

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P0_1);
AnalogInputPin cds(FEHIO::P0_2);
DigitalInputPin microswitch(FEHIO::P0_3);

FEHMotor left_motor(FEHMotor::Motor0);
FEHMotor right_motor(FEHMotor::Motor1);
FEHMotor vex_motor(FEHMotor::Motor2);

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
            right_motor.SetPercent(-MOTOR_PERCENT);
            left_motor.SetPercent(-MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.X() < x_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(MOTOR_PERCENT);
            left_motor.SetPercent(MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }
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
            right_motor.SetPercent(MOTOR_PERCENT);
            left_motor.SetPercent(MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.X() < x_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(-MOTOR_PERCENT);
            left_motor.SetPercent(-MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }
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
            right_motor.SetPercent(MOTOR_PERCENT);
            left_motor.SetPercent(MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.Y() < y_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(-MOTOR_PERCENT);
            left_motor.SetPercent(-MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }
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
            right_motor.SetPercent(-MOTOR_PERCENT);
            left_motor.SetPercent(-MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
        else if(RPS.Y() < y_coordinate)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            right_motor.SetPercent(MOTOR_PERCENT);
            left_motor.SetPercent(MOTOR_PERCENT);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            Sleep(100);
        }
    }
}

void check_heading(DriveTrain& driveTrain, float heading) //using RPS
{
    LCD.Write("Checking heading: ");
    LCD.WriteLine(heading);

    if (heading == 0)
    {
        while (RPS.Heading() < 2 || RPS.Heading() > 358)
        {
            if (RPS.Heading() >= 180)
            {
                driveTrain.Turn(LRDirection::Left, 10, 3);
            }
            else
            {
                driveTrain.Turn(LRDirection::Right, 10, 3);
            }
        }
    }

    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)
    while (RPS.Heading() < heading - 0.5 || RPS.Heading() > heading + 0.5)
    {
        if (RPS.Heading() > heading)
        {
            LCD.WriteLine("Too counterclockwise!");
            driveTrain.Turn(LRDirection::Right, 20, 3);
        }
        else if (RPS.Heading() < heading)
        {
            LCD.WriteLine("Too clockwise!");
            driveTrain.Turn(LRDirection::Left, 20, 3);
        }
        Sleep(500);
    }
}

void col_turn_clockwise()
{
    right_motor.SetPercent(35);
    left_motor.SetPercent(35);
    Sleep(100);
    vex_motor.SetPercent(50);
    Sleep(3000);
    vex_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

void col_turn_cclockwise()
{
    right_motor.SetPercent(35);
    left_motor.SetPercent(35);
    Sleep(100);
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

    LCD.WriteLine("Before DTrain");
    DriveTrain *driveTrain = new DriveTrain(left_motor, right_motor, left_encoder, right_encoder);

    LCD.WriteLine("Press middle button");
    while(!buttons.MiddlePressed()); //Wait for middle button to be pressed
    while(buttons.MiddlePressed()); //Wait for middle button to be unpressed
    RPS.InitializeMenu();

//    LCD.WriteLine("Waiting for light...");
//    while (cds.Value() > 0.17);
//    while (true)
//    {
//        LCD.Write("(");
//        LCD.Write(RPS.X());
//        LCD.Write(",");
//        LCD.Write(RPS.Y());
//        LCD.Write(",");
//        LCD.Write(RPS.Heading());
//        LCD.WriteLine(")");
//        Sleep(1000);
//    }

    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 14.0f);

    LCD.Write("Heading: ");
    LCD.WriteLine(RPS.Heading());

    Sleep(1000);
    check_y_minus(15.699);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT);
    LCD.Write("Heading: ");
    LCD.WriteLine(RPS.Heading());

    Sleep(1000);
    check_heading(*driveTrain, 90);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 10.0f);
    LCD.Write("Heading: ");
    LCD.WriteLine(RPS.Heading());

    Sleep(1000);
    check_x_plus(32.5);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT);
    LCD.Write("Heading: ");
    LCD.WriteLine(RPS.Heading());

    Sleep(1000);
    check_heading(*driveTrain, 179);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 4.0f);
    LCD.Write("Heading: ");
    LCD.WriteLine(RPS.Heading());
//    check_y_plus(23.401);
//    check_heading(heading);
    Sleep(1000);
    check_heading(*driveTrain, 178);
    driveTrain->Drive(FBDirection::Forward, 60, 20.0f);
    Sleep(1000);

    // Go to the buttons
    check_heading(*driveTrain, 180);
    check_y_plus(56);
    Sleep(1000);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT);
    check_heading(*driveTrain, 270);
    Sleep(1000);
    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 12.0f);
    check_x_minus(21);
    Sleep(1000);
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT, 55);
    check_heading(*driveTrain, 230);
    Sleep(1000);
    driveTrain->Drive(FBDirection::Forward, 50, 5.0f);
    check_heading(*driveTrain, 230);
    Sleep(1000);
    driveTrain->Drive(FBDirection::Forward, 50, 1.0f);
    check_heading(*driveTrain, 230);

    press_buttons(vex_motor);
    LCD.WriteLine("Done!!!");

    delete driveTrain;
    return 0;
}

