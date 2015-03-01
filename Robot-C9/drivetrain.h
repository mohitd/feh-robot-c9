#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <FEHMotor.h>
#include <FEHIO.h>

struct Direction
{
    enum Type
    {
        Left, Right
    };
    Type type;
    Direction(Type t) : type(t) { }
    operator Type() const { return type; }
private:
    template<typename T>
    operator T () const;
};

class DriveTrain
{
public:
    DriveTrain(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder);
    ~DriveTrain();

    void Drive(int power, float distance);
    void Drive(int power, int counts);
    void Turn(Direction direction, int power, int counts);
    void Turn(Direction direction, int power);

private:
    FEHMotor leftMotor, rightMotor;
    DigitalEncoder leftEncoder, rightEncoder;

    void ResetCounts();
};

#endif // DRIVETRAIN_H
