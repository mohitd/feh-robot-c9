#include "diagnostics.h"

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHLCD.h>
#include <FEHUtility.h>

void Diagnostics::RunDiagnostics()
{
    LCD.WriteLine("Runnign Diagnostics...");
    TestDriveForward();
    while (!buttonBoard.MiddlePressed());
    while (buttonBoard.MiddlePressed());
    TestDriveBackward();
    while (!buttonBoard.MiddlePressed());
    while (buttonBoard.MiddlePressed());
    TestCdSCell();
    while (!buttonBoard.MiddlePressed());
    while (buttonBoard.MiddlePressed());
    TestSwitches();
    while (!buttonBoard.MiddlePressed());
    while (buttonBoard.MiddlePressed());
    LCD.Clear();
}

void Diagnostics::TestDriveForward()
{
    LCD.WriteLine("Driving Forward...");
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    leftMotor.SetPercent(40);
    rightMotor.SetPercent(40);

    while ((leftEncoder.Counts() + rightEncoder.Counts()) / 2. < 33.74 * 12);

    leftMotor.Stop();
    rightMotor.Stop();

    LCD.Write("LE: ");
    LCD.WriteLine(leftEncoder.Counts());
    LCD.Write("RE: ");
    LCD.WriteLine(rightEncoder.Counts());
}

void Diagnostics::TestDriveBackward()
{
    LCD.WriteLine("Driving Backward...");
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    leftMotor.SetPercent(-40);
    rightMotor.SetPercent(-40);

    while ((leftEncoder.Counts() + rightEncoder.Counts()) / 2. < 33.74 * 12);

    leftMotor.Stop();
    rightMotor.Stop();

    LCD.Write("LE: ");
    LCD.WriteLine(leftEncoder.Counts());
    LCD.Write("RE: ");
    LCD.WriteLine(rightEncoder.Counts());
}

void Diagnostics::TestCdSCell()
{
    LCD.WriteLine("Testing CdS cell...");
    LCD.WriteLine("Press M to stop");

    while (!buttonBoard.MiddlePressed())
    {
        LCD.Write("CdS: ");
        LCD.WriteLine(cds.Value());
        Sleep(1000);
    }
    LCD.Clear();
}

void Diagnostics::TestSwitches()
{
    LCD.WriteLine("Testing switches...");
    bool switch1 = false, switch2 = false, switch3 = false;

    if (cage.Value() == 1) LCD.WriteLine("CAGE NOT PRESSED!");
    else switch1 = true;
    if (crank.Value() == 0) LCD.WriteLine("CRANK PRESSED!");
    else switch2 = true;
    if (col.Value() == 0) LCD.WriteLine("COL PRESSED");
    else switch3 = true;

    if (switch1 && switch2 && switch3) LCD.WriteLine("All good!");

    LCD.WriteLine("Unpress cage");
    while (cage.Value() == 0);
    LCD.WriteLine("Cage Ok!");

    LCD.WriteLine("Press crank");
    while (crank.Value() == 1);
    LCD.WriteLine("Crank Ok!");

    LCD.WriteLine("Press col");
    while (col.Value() == 1);
    LCD.WriteLine("Col Ok!");
    LCD.WriteLine("\n");
    LCD.WriteLine("All switches work!");
}
