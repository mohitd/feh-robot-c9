#include <FEHIO.h>
#include <FEHLCD.h>
#include <cstring>
#include "logger.h"

void Logger::Display(ButtonBoard board)
{

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
