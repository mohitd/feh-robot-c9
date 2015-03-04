#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include "logger.h"

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
    DriveTrain(Logger& logger, FEHMotor& leftMotor, FEHMotor& rightMotor, DigitalEncoder& leftEncoder, DigitalEncoder& rightEncoder)
        : logger(logger), leftMotor(leftMotor), rightMotor(rightMotor), leftEncoder(leftEncoder), rightEncoder(rightEncoder)
    {

    }

    void Drive(FBDirection direction, int power, float distance);
    void Drive(FBDirection direction, int power, int counts);
    void Turn(LRDirection direction, int power, int counts);
    void Turn(LRDirection direction, int power);

private:
    FEHMotor& leftMotor;
    FEHMotor& rightMotor;
    DigitalEncoder& leftEncoder;
    DigitalEncoder& rightEncoder;
    Logger& logger;

    void ResetCounts();
};

#endif // DRIVETRAIN_H
