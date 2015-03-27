#include "drivetrain.h"

#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHRPS.h>
#include <cstring>
#include <cmath>

#define M_PI 3.14159265358979323846
#define COUNTS_PER_90_DEGREE 204
#define COUNTS_PER_INCH 33.74
#define INCH_TO_COUNTS(X) ((X) * COUNTS_PER_INCH)
#define MOTOR_CORRECTION 1
#define MOTOR_PERCENT_CHECKS 25
#define TIMEOUT 5

using namespace std;
void DriveTrain::Drive(FBDirection direction, float percent, int counts)
{
    Drive(direction, percent, percent, counts);
}

void DriveTrain::Drive(FBDirection direction, float percent, float distance)
{
    Drive(direction, percent, (int)INCH_TO_COUNTS(distance));
}

void DriveTrain::Drive(FBDirection direction, float leftPercent, float rightPercent, float distance)
{
    Drive(direction, leftPercent, rightPercent, (int)INCH_TO_COUNTS(distance));
}

void DriveTrain::Drive(FBDirection direction, float leftPercent, float rightPercent, int counts)
{
    ResetCounts();
    switch (direction)
    {
    case FBDirection::Forward:
        leftMotor.SetPercent(leftPercent);
        rightMotor.SetPercent(rightPercent);
        break;
    case FBDirection::Backward:
        leftMotor.SetPercent(-leftPercent);
        rightMotor.SetPercent(-rightPercent);
        break;
    }

    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts);
    leftMotor.Stop();
    rightMotor.Stop();
    Sleep(100);
}

void DriveTrain::DriveToCoord(float x, float y, bool front, float percent)
{
    float heading;
    if (front) heading = atan2(y, x);
    else heading = atan2(y, x) + M_PI;

    Sleep(100);
    CheckHeading(heading);
    Sleep(100);
    if (front)
    {
        rightMotor.SetPercent(percent);
        leftMotor.SetPercent(percent);
    }
    else
    {
        rightMotor.SetPercent(-percent);
        leftMotor.SetPercent(-percent);
    }

    while (abs(RPS.X() - x) > 1 && abs(RPS.Y() - y) > 1)
    {
        LCD.Clear();
        LCD.Write("(");
        LCD.Write(RPS.X());
        LCD.Write(",");
        LCD.Write(RPS.Y());
        LCD.Write(",");
        LCD.Write(RPS.Heading());
        LCD.WriteLine(")");
        Sleep(500);
    }
    rightMotor.Stop();
    leftMotor.Stop();
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
    Sleep(100);
}

void DriveTrain::Turn(LRDirection direction, float percent)
{
    Turn(direction, percent, COUNTS_PER_90_DEGREE);
}
void DriveTrain::Accelerate(FBDirection direction, float startPercent, float endPercent, float time)
{
    int interval = (endPercent - startPercent) / time;
    int sum = 0;
    if (direction == FBDirection::Forward)
    {
        leftMotor.SetPercent(startPercent);
        rightMotor.SetPercent(startPercent);
    }
    else
    {
        leftMotor.SetPercent(-startPercent);
        rightMotor.SetPercent(-startPercent);
    }

    ResetTime();
    while (TimeNowMSec() <= time*1000)
    {
        if (TimeNowMSec() % 1000 == 0)
        {
            if (direction == FBDirection::Forward)
            {
                leftMotor.SetPercent(startPercent + sum);
                rightMotor.SetPercent(startPercent + sum);
            }
            else
            {
                leftMotor.SetPercent(-(startPercent + sum));
                rightMotor.SetPercent(-(startPercent + sum));
            }
            sum += interval;
        }
    }

    leftMotor.Stop();
    rightMotor.Stop();
}

void DriveTrain::CheckX(float x, bool facingPlus)
{
    LCD.Write("Checking x coord: ");
    LCD.WriteLine(x);

    //check whether the robot is within an acceptable range
    while(abs(x - RPS.X()) > 1)
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
}

void DriveTrain::CheckY(float y, bool facingPlus)
{
    LCD.Write("Checking y coord: ");
    LCD.WriteLine(y);

    //check whether the robot is within an acceptable range
    while(abs(y - RPS.Y()) > 1)
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
}

void DriveTrain::CheckHeading(float heading)
{
    LCD.Write("Checking heading: ");
    LCD.WriteLine(heading);

    int start = TimeNowSec();
    if (heading == 0)
    {
        while ((RPS.Heading() < 358 || RPS.Heading() > 2) && (TimeNowSec() - start < TIMEOUT))
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
        while ((RPS.Heading() < heading - 2 || RPS.Heading() > heading + 2) && (TimeNowSec() - start < TIMEOUT))
        {
            if (abs(RPS.Heading() - heading) >= 180)
            {
                if (RPS.Heading() > heading)
                {
                    LCD.WriteLine("Too counterclockwise!");
                    Turn(LRDirection::Left, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
                }
                else if (RPS.Heading() < heading)
                {
                    LCD.WriteLine("Too clockwise!");
                    Turn(LRDirection::Right, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
                }
            }
            else
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
}

void DriveTrain::ResetCounts()
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
}
