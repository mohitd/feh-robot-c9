#include "drivetrain.h"

#define COUNTS_PER_90_DEGREE 204
#define COUNTS_PER_INCH 33.74
#define INCH_TO_COUNTS(X) ((X) * COUNTS_PER_INCH)

DriveTrain::DriveTrain(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder)
{
    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;
    this->leftEncoder = leftEncoder;
    this->rightEncoder = rightEncoder;
}

void DriveTrain::Drive(int power, int counts)
{
    leftMotor.SetPower(power);
    rightMotor.SetPower(power);
    ResetCounts();
    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts);
    leftMotor.Stop();
    rightMotor.Stop();
}

void DriveTrain::Drive(int power, float distance)
{
    Drive(power, INCH_TO_COUNTS(distance));
}

void DriveTrain::Turn(Direction direction, int power, int counts)
{
    ResetCounts();
    switch (direction) {
    case Direction::Left:
        leftMotor.SetPower(-power);
        rightMotor.SetPower(power);
        break;
    case Direction::Right:
        leftMotor.SetPower(power);
        rightMotor.SetPower(-power);
        break;
    default:
        break;
    }

    while ((leftEncoder.Counts() + rightEncoder.Counts()) * .5 < counts);
    leftMotor.Stop();
    rightMotor.Stop();
}

void DriveTrain::Turn(Direction direction, int power)
{
    Turn(direction, power, COUNTS_PER_90_DEGREE);
}

void DriveTrain::ResetCounts()
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
}
