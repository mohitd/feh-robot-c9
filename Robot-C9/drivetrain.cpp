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
#define RAD_TO_DEG(X) ((X) * 180 / M_PI)
#define MOTOR_CORRECTION 3
#define MOTOR_PERCENT_CHECKS 30
#define TIMEOUT 5

using namespace std;

void DriveTrain::Drive(FBDirection direction, float percent)
{
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
}

void DriveTrain::Stop()
{
    leftMotor.Stop();
    rightMotor.Stop();
}

void DriveTrain::Drive(FBDirection direction, float percent, int counts)
{
    Drive(direction, percent, percent, counts);
}

void DriveTrain::Drive(FBDirection direction, float percent, float distance)
{
    LCD.Write("Driving ");
    LCD.WriteLine(distance);
    Drive(direction, percent, (int)INCH_TO_COUNTS(distance));
}

void DriveTrain::Drive(FBDirection direction, float leftPercent, float rightPercent, float distance)
{
    LCD.Write("Driving ");
    LCD.WriteLine(distance);
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

    int start = TimeNowSec();
    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts && TimeNowSec() - start < TIMEOUT);
    leftMotor.Stop();
    rightMotor.Stop();
    Sleep(100);
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

    int start = TimeNowSec();
    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts && TimeNowSec() - start < TIMEOUT);
    leftMotor.Stop();
    rightMotor.Stop();
    Sleep(100);
}

void DriveTrain::Turn(LRDirection direction, float percent)
{
    Turn(direction, percent, COUNTS_PER_90_DEGREE);
}

/*
 * Never gonna give you up
 * Never gonna let you down
 * Never gonna run around and desert you
 * Never gonna make you cry
 * Never gonna say goodbye
 * Never gonna tell a lie and hurt you
 */

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

// ...Bet that's still stuck in your head...

void DriveTrain::CheckX(float x, bool facingPlus)
{
    LCD.Write("Checking Y from ");
    LCD.Write(RPS.X());
    LCD.Write(" to ");
    LCD.WriteLine(x);

    //check whether the robot is within an acceptable range
    int start = TimeNowSec();
    while(abs(x - RPS.X()) > .5  && (TimeNowSec() - start < TIMEOUT))
    {
        if (RPS.X() < 0)
        {
            LCD.SetFontColor(FEHLCD::Red);
            LCD.WriteLine("!!!RPS DOWN!!!");
            LCD.SetFontColor(FEHLCD::Green);
            continue;
        }
        if(RPS.X() > x)
        {
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
    LCD.Write("Checking Y from ");
    LCD.Write(RPS.Y());
    LCD.Write(" to ");
    LCD.WriteLine(y);

    //check whether the robot is within an acceptable range
    int start = TimeNowSec();
    while(abs(y - RPS.Y()) > .5  && (TimeNowSec() - start < TIMEOUT))
    {
        if (RPS.Y() < 0)
        {
            LCD.SetFontColor(FEHLCD::Red);
            LCD.WriteLine("!!!RPS DOWN!!!");
            LCD.SetFontColor(FEHLCD::Green);
            continue;
        }
        if(RPS.Y() > y)
        {
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
    LCD.Write("Checking T from ");
    LCD.Write(RPS.Heading());
    LCD.Write(" to ");
    LCD.WriteLine(heading);

    int start = TimeNowSec();
    while (abs(RPS.Heading() - heading) > 1 && (TimeNowSec() - start < TIMEOUT))
    {
        if (RPS.Heading() < 0)
        {
            LCD.SetFontColor(FEHLCD::Red);
            LCD.WriteLine("!!!RPS DOWN!!!");
            LCD.SetFontColor(FEHLCD::Green);
            continue;
        }
        if (abs(RPS.Heading() - heading) >= 180)
        {
            if (RPS.Heading() > heading)
            {
                Turn(LRDirection::Left, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
            }
            else if (RPS.Heading() < heading)
            {
                Turn(LRDirection::Right, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
            }
        }
        else
        {
            if (RPS.Heading() > heading)
            {
                Turn(LRDirection::Right, MOTOR_PERCENT_CHECKS, MOTOR_CORRECTION);
            }
            else if (RPS.Heading() < heading)
            {
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
