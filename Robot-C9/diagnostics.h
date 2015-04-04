#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include "robot.h"

class Diagnostics
{
public:
    Diagnostics(ButtonBoard &buttonBoard, FEHMotor &leftMotor, FEHMotor &rightMotor, DigitalEncoder &leftEncoder, DigitalEncoder &rightEncoder, AnalogInputPin &cds,
                DigitalInputPin &cage, DigitalInputPin &col, DigitalInputPin &crank)
        : buttonBoard(buttonBoard), leftMotor(leftMotor), rightMotor(rightMotor), leftEncoder(leftEncoder), rightEncoder(rightEncoder), cds(cds), cage(cage), col(col), crank(crank)
    {

    }

    void RunDiagnostics();
private:
    ButtonBoard &buttonBoard;
    FEHMotor &leftMotor, &rightMotor;
    DigitalEncoder &leftEncoder, &rightEncoder;
    AnalogInputPin &cds;
    DigitalInputPin &cage, &col, &crank;

    void TestDriveForward();
    void TestDriveBackward();
    void TestCdSCell();
    void TestSwitches();
    // TODO: Cage down/up, Cols, Crank
};

#endif // DIAGNOSTICS_H
