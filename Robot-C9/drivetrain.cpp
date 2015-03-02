#include "drivetrain.h"

#define COUNTS_PER_90_DEGREE 204
#define COUNTS_PER_INCH 33.74
#define INCH_TO_COUNTS(X) ((X) * COUNTS_PER_INCH)

DriveTrain::DriveTrain(FEHMotor *leftMotor, FEHMotor *rightMotor, DigitalEncoder *leftEncoder, DigitalEncoder *rightEncoder)
{
    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;
    this->leftEncoder = leftEncoder;
    this->rightEncoder = rightEncoder;
}

void DriveTrain::Drive(FBDirection direction, int power, int counts)
{
    ResetCounts();
    switch (direction) {
    case FBDirection::Forward:
        leftMotor->SetPower(power);
        rightMotor->SetPower(power);
        break;
    case FBDirection::Backward:
        leftMotor->SetPower(-power);
        rightMotor->SetPower(-power);
        break;
    default:
        break;
    }
    while ((leftEncoder->Counts() + rightEncoder->Counts()) * .5 < counts);
    leftMotor->Stop();
    rightMotor->Stop();
}

void DriveTrain::Drive(FBDirection direction, int power, float distance)
{
    Drive(direction, power, INCH_TO_COUNTS(distance));
}

void DriveTrain::Turn(LRDirection direction, int power, int counts)
{
    ResetCounts();
    switch (direction) {
    case LRDirection::Left:
        leftMotor->SetPower(-power);
        rightMotor->SetPower(power);
        break;
    case LRDirection::Right:
        leftMotor->SetPower(power);
        rightMotor->SetPower(-power);
        break;
    default:
        break;
    }

    while ((leftEncoder->Counts() + rightEncoder->Counts()) * .5 < counts);
    leftMotor->Stop();
    rightMotor->Stop();
}

void DriveTrain::Turn(LRDirection direction, int power)
{
    Turn(direction, power, COUNTS_PER_90_DEGREE);
}

void DriveTrain::ResetCounts()
{
    leftEncoder->ResetCounts();
    rightEncoder->ResetCounts();
}
