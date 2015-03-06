#ifndef LOGGER_H
#define LOGGER_H

#include <FEHIO.h>

struct Log_t
{
    float x, y, heading;
    char function[40];
    char message[40];
};

class Logger
{
public:
    Logger() : i(0)
    {

    }

    void Display(ButtonBoard board);
    void Log(Log_t item);

private:
    int i;
    Log_t array[10];
};

#endif // LOGGER_H
