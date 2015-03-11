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

void DriveTrain::ResetCounts()
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
}
