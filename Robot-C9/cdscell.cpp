#include "cdscell.h"
#include <FEHLCD.h>
#include <FEHUtility.h>

#define LIGHT_THRESHOLD 1
#define COLOR_THRESHOLD 0.5
#define TIMEOUT 5

void CdSCell::WaitForLight()
{
    LCD.WriteLine("Waiting for light...");
    int start = TimeNowSec();
    while (cds.Value() > LIGHT_THRESHOLD && (start - TimeNow()) < TIMEOUT);
}

Color CdSCell::DetectColor()
{
    LCD.WriteLine("Detecting color...");
    if (cds.Value() > COLOR_THRESHOLD) return Color::Blue;
    else return Color::Red;
}
