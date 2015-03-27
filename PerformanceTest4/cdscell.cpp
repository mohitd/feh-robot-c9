#include "cdscell.h"

#define LIGHT_THRESHOLD 1
#define COLOR_THRESHOLD 0.5

void CdSCell::WaitForLight()
{
    while (cds.Value() > LIGHT_THRESHOLD);
}

Color CdSCell::DetectColor()
{
    if (cds.Value() > COLOR_THRESHOLD) return Color::Blue;
    else return Color::Red;
}
