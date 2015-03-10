#include "drivetrain.h"

#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHIO.h>
#include "logger.h"

#define COUNTS_PER_90_DEGREE 204
#define COUNTS_PER_INCH 33.74
#define INCH_TO_COUNTS(X) ((X) * COUNTS_PER_INCH)
#define MOTOR_CORRECTION 3

void DriveTrain::Drive(FBDirection direction, float percent, int counts)
{
    ResetCounts();
    leftMotor.SetPercent(direction == FBDirection::Forward ? percent : -percent);
    rightMotor.SetPercent(direction == FBDirection::Forward ? percent : -percent);

    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts)
    {
        if (leftEncoder.Counts() > rightEncoder.Counts())
        {
            rightMotor.SetPercent(percent + (direction == FBDirection::Forward ? MOTOR_CORRECTION : -MOTOR_CORRECTION));
        }
        if (rightEncoder.Counts() > leftEncoder.Counts())
        {
            leftMotor.SetPercent(percent + (direction == FBDirection::Forward ? MOTOR_CORRECTION : -MOTOR_CORRECTION));
        }
        if (rightEncoder.Counts() == leftEncoder.Counts())
        {
            leftMotor.SetPercent(direction == FBDirection::Forward ? percent : -percent);
            rightMotor.SetPercent(direction == FBDirection::Forward ? percent : -percent);
        }

        Sleep(100);
    }
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
    leftMotor.SetPercent(direction == LRDirection::Left ? -percent : percent);
    rightMotor.SetPercent(direction == LRDirection::Left ? percent : -percent);

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
