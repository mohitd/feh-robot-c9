#include "cdscell.h"
#include <FEHLCD.h>

#define LIGHT_THRESHOLD 1
#define COLOR_THRESHOLD 0.5

void CdSCell::WaitForLight()
{
    LCD.WriteLine("Waiting for light...");
    while (cds.Value() > LIGHT_THRESHOLD);
}

Color CdSCell::DetectColor()
{
    LCD.WriteLine("Detecting color...");
    if (cds.Value() > COLOR_THRESHOLD) return Color::Blue;
    else return Color::Red;
}
