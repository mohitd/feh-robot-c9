#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <cmath>

#include "drivetrain.h"
#include "cdscell.h"
#include "cage.h"

#define TIMEOUT 5
#define MOTOR_PERCENT 40

using namespace std;

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P0_1);
AnalogInputPin cds(FEHIO::P1_0);
DigitalInputPin cage_microswitch(FEHIO::P2_0);
DigitalInputPin col_microswitch(FEHIO::P2_1);
DigitalInputPin crank_microswitch(FEHIO::P2_2);

FEHMotor left_motor(FEHMotor::Motor0);
FEHMotor right_motor(FEHMotor::Motor1);
FEHMotor col_motor(FEHMotor::Motor2);
FEHMotor cage_motor(FEHMotor::Motor3);


int main(void)
{
    ButtonBoard buttons( FEHIO::Bank3 );

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    DriveTrain *driveTrain = new DriveTrain(left_motor, right_motor, left_encoder, right_encoder);
    CdSCell *cdsCell = new CdSCell(cds);
    Cage *cage = new Cage(cage_motor, cage_microswitch);

    LCD.WriteLine("Press middle button");
    while(!buttons.MiddlePressed()); //Wait for middle button to be pressed
    while(buttons.MiddlePressed()); //Wait for middle button to be unpressed

    LCD.WriteLine("Did you initialize the column? Y(R) N(L)");
    while (!buttons.RightPressed());
    while (buttons.RightPressed());
    LCD.WriteLine("Good!");

    RPS.InitializeMenu();

    LCD.WriteLine("Waiting for light...");
    cdsCell->WaitForLight();

    driveTrain->Drive(FBDirection::Forward, MOTOR_PERCENT, 14.0f);
    driveTrain->CheckY(18.5, false);
    cage->Lower();
    driveTrain->Turn(LRDirection::Right, MOTOR_PERCENT);
    driveTrain->CheckHeading(270);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 8.0f);
    driveTrain->CheckX(28.5, false);
    driveTrain->Turn(LRDirection::Left, MOTOR_PERCENT);
    driveTrain->CheckHeading(0);
    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 8.0f);
    driveTrain->CheckY(24.0, false);
    driveTrain->CheckHeading(0);
    driveTrain->Accelerate(FBDirection::Backward, 30, 65, 1.5f);
    driveTrain->CheckY(43, false);
    driveTrain->CheckHeading(0);

    cage->RaiseHalf();

    if (RPS.X() <= 30) driveTrain->CheckHeading(340);
    else if (RPS.X() >= 32) driveTrain->CheckHeading(20);
    while (abs(RPS.X() - 31) > 1) driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 5);
    driveTrain->CheckHeading(0);

    driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 4.0f);

    if (RPS.X() <= 30) driveTrain->CheckHeading(340);
    else if (RPS.X() >= 32) driveTrain->CheckHeading(20);
    while (abs(RPS.X() - 31) > 1) driveTrain->Drive(FBDirection::Backward, MOTOR_PERCENT, 5);
    driveTrain->CheckHeading(0);

    driveTrain->CheckY(57, false);
    if (cdsCell->DetectColor() == Color::Blue)
    {
        // Blue
        cage_motor.SetPercent(-30);
        Sleep(2000);
        cage_motor.Stop();
    }
    else
    {
        // Red
        cage_motor.SetPercent(30);
        Sleep(2000);
        cage_motor.Stop();
    }

    LCD.WriteLine("Done!!!");

    return 0;
}

