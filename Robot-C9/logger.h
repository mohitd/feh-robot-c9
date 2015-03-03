#ifndef LOGGER_H
#define LOGGER_H

#include <FEHIO.h>

struct Log_t
{
    float x, y, heading;
};

class Logger
{
public:
    Logger();
    void Display(ButtonBoard board);

private:
    int i = 0;
    Log_t array[1000];
};

#endif // LOGGER_H
