#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <FEHMotor.h>
#include <FEHIO.h>

class Diagnostics
{
public:
    Diagnostics(ButtonBoard &buttonBoard, FEHMotor &leftMotor, FEHMotor &rightMotor, FEHMotor &cageMotor, FEHMotor colMotor,
                DigitalEncoder &leftEncoder, DigitalEncoder &rightEncoder, AnalogInputPin &cds, DigitalInputPin &cage, DigitalInputPin &col, DigitalInputPin &crank)
        : buttonBoard(buttonBoard), leftMotor(leftMotor), rightMotor(rightMotor), cageMotor(cageMotor), colMotor(colMotor),
          leftEncoder(leftEncoder), rightEncoder(rightEncoder), cds(cds), cageSwitch(cage), colSwitch(col), crankSwitch(crank)
    {

    }

    void RunDiagnostics();
private:
    ButtonBoard &buttonBoard;
    FEHMotor &leftMotor, &rightMotor, &cageMotor, &colMotor;
    DigitalEncoder &leftEncoder, &rightEncoder;
    AnalogInputPin &cds;
    DigitalInputPin &cageSwitch, &colSwitch, &crankSwitch;

    void TestDriveForward();
    void TestDriveBackward();
    void TestCdSCell();
    void TestSwitches();
    void TestCage();
    // TODO: Cage down/up, Cols, Crank
};

#endif // DIAGNOSTICS_H
