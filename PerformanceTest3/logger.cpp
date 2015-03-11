#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <cstring>
#include "logger.h"

void Logger::Display(ButtonBoard& buttons)
{
    LCD.Clear();
    LCD.WriteLine("Press the L and R buttons");
    LCD.WriteLine("to cycle through logs");
    LCD.WriteLine("M to continue/exit");
    while (!buttons.MiddlePressed());
    while (buttons.MiddlePressed());

    int size = i;
    i = 0;
    DisplayLogItem(array[i]);

    while (true)
    {
        if (buttons.RightPressed())
        {
            i++;
            if (i > size) i = size;
            DisplayLogItem(array[i]);
        }
        if (buttons.LeftPressed())
        {
            i--;
            if (i < 0) i = 0;
            DisplayLogItem(array[i]);
        }
        Sleep(500);
    }
}

void Logger::Log(Log_t item)
{
    array[i].x = item.x;
    array[i].y = item.y;
    array[i].heading = item.heading;
    std::strcpy(array[i].function, item.function);
    std::strcpy(array[i].message, item.message);
    i++;
}

void Logger::DisplayLogItem(Log_t item)
{
    LCD.Clear();
    LCD.Write("X: ");
    LCD.WriteLine(item.x);
    LCD.Write("Y: ");
    LCD.WriteLine(item.y);
    LCD.Write("H: ");
    LCD.WriteLine(item.heading);
    LCD.WriteLine(item.function);
    LCD.WriteLine(item.message);
}
