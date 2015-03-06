#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <FEHMotor.h>
#include <FEHIO.h>

struct LRDirection
{
    enum Type
    {
        Left, Right
    };
    Type type;
    LRDirection(Type t) : type(t) { }
    operator Type() const { return type; }
private:
    template<typename T>
    operator T () const;
};

struct FBDirection
{
    enum Type
    {
        Forward, Backward
    };
    Type type;
    FBDirection(Type t) : type(t) { }
    operator Type() const { return type; }
private:
    template<typename T>
    operator T () const;
};

class DriveTrain
{
public:
    DriveTrain(FEHMotor& leftMotor, FEHMotor& rightMotor, DigitalEncoder& leftEncoder, DigitalEncoder& rightEncoder)
        : leftMotor(leftMotor), rightMotor(rightMotor), leftEncoder(leftEncoder), rightEncoder(rightEncoder)
    {

    }

    void Drive(FBDirection direction, float percent, float distance);
    void Drive(FBDirection direction, float percent, int counts);
    void Turn(LRDirection direction, float percent, int counts);
    void Turn(LRDirection direction, float percent);

private:
    FEHMotor& leftMotor;
    FEHMotor& rightMotor;
    DigitalEncoder& leftEncoder;
    DigitalEncoder& rightEncoder;

    void ResetCounts();
};

#endif // DRIVETRAIN_H
