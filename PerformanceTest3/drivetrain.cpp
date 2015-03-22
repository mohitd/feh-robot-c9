#include "drivetrain.h"

#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHRPS.h>
#include <cstring>
#include "logger.h"

#define COUNTS_PER_90_DEGREE 204
#define COUNTS_PER_INCH 33.74
#define INCH_TO_COUNTS(X) ((X) * COUNTS_PER_INCH)
#define MOTOR_CORRECTION 3
#define MOTOR_PERCENT_CHECKS 25
#define TIMEOUT 5

using namespace std;
void DriveTrain::Drive(FBDirection direction, float percent, int counts)
{
    ResetCounts();
    switch (direction)
    {
    case FBDirection::Forward:
        leftMotor.SetPercent(percent);
        rightMotor.SetPercent(percent);
        break;
    case FBDirection::Backward:
        leftMotor.SetPercent(-percent);
        rightMotor.SetPercent(-percent);
        break;
    }

    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts);
    leftMotor.Stop();
    rightMotor.Stop();
    Sleep(500);
}

void DriveTrain::Drive(FBDirection direction, float percent, float distance)
{
    Drive(direction, percent, (int)INCH_TO_COUNTS(distance));
}

void DriveTrain::Turn(LRDirection direction, float percent, int counts)
{
    ResetCounts();
    switch (direction)
    {
    case LRDirection::Left:
        leftMotor.SetPercent(-percent);
        rightMotor.SetPercent(percent);
        break;
    case LRDirection::Right:
        leftMotor.SetPercent(percent);
        rightMotor.SetPercent(-percent);
        break;
    }

    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts);
    leftMotor.Stop();
    rightMotor.Stop();
    Sleep(500);
}

void DriveTrain::Turn(LRDirection direction, float percent)
{
    Turn(direction, percent, COUNTS_PER_90_DEGREE);
}

void DriveTrain::CheckX(float x, bool facingPlus)
{
    LCD.Write("Checking x coord: ");
    LCD.WriteLine(x);

    //check whether the robot is within an acceptable range
    while(RPS.X() < x - 1 || RPS.X() > x + 1)
    {
        if(RPS.X() > x)
        {
            LCD.WriteLine("Too far!");
            //pulse the motors for a short duration in the correct direction
            if (facingPlus)
            {
                rightMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
            }
            else
            {
                rightMotor.SetPercent(MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(MOTOR_PERCENT_CHECKS);
            }
            Sleep(100);
            rightMotor.Stop();
            leftMotor.Stop();
            Sleep(100);
        }
        else if(RPS.X() < x)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            if (facingPlus)
            {
                rightMotor.SetPercent(MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(MOTOR_PERCENT_CHECKS);                
            }
            else
            {
                rightMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
            }

            Sleep(100);
            rightMotor.Stop();
            leftMotor.Stop();
            Sleep(100);
        }
    }

    Sleep(500);
}

void DriveTrain::CheckY(float y, bool facingPlus)
{
    LCD.Write("Checking y coord: ");
    LCD.WriteLine(y);

    //check whether the robot is within an acceptable range
    while(RPS.Y() < y - 1 || RPS.Y() > y + 1)
    {
        if(RPS.Y() > y)
        {
            LCD.WriteLine("Too far!");
            //pulse the motors for a short duration in the correct direction
            if (facingPlus)
            {
                rightMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
            }
            else
            {
                rightMotor.SetPercent(MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(MOTOR_PERCENT_CHECKS);
            }
            Sleep(100);
            rightMotor.Stop();
            leftMotor.Stop();
            Sleep(100);
        }
        else if(RPS.Y() < y)
        {
            LCD.WriteLine("Too short!");
            //pulse the motors for a short duration in the correct direction
            if (facingPlus)
            {
                rightMotor.SetPercent(MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(MOTOR_PERCENT_CHECKS);                
            }
            else
            {
                rightMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
                leftMotor.SetPercent(-MOTOR_PERCENT_CHECKS);
            }

            Sleep(100);
            rightMotor.Stop();
            leftMotor.Stop();
            Sleep(100);
        }
    }

    Sleep(500);
}

void DriveTrain::CheckHeading(float heading)
{
    LCD.Write("Checking heading: ");
    LCD.WriteLine(heading);

    int start = TimeNowSec();
    if (heading == 0)
    {
        while ((RPS.Heading() < 358 || RPS.Heading() > 2) && (TimeNow() - start < TIMEOUT))
        {
            if (RPS.Heading() >= 180)
            {
                Turn(LRDirection::Left, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
            }
            else
            {
                Turn(LRDirection::Right, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
            }
            LCD.Write("Heading: ");
            LCD.WriteLine(RPS.Heading());
        }
    }
    else
    {
        while (RPS.Heading() < heading - 2 || RPS.Heading() > heading + 2)
        {
            if (RPS.Heading() > heading)
            {
                LCD.WriteLine("Too counterclockwise!");
                Turn(LRDirection::Right, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
            }
            else if (RPS.Heading() < heading)
            {
                LCD.WriteLine("Too clockwise!");
                Turn(LRDirection::Left, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
            }
        }
    }
}

void DriveTrain::ResetCounts()
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
}
