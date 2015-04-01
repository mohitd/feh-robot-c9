#ifndef CDSCELL_H
#define CDSCELL_H

#include <FEHIO.h>
#include <FEHLCD.h>

struct Color
{
    enum Type
    {
        Blue, Red
    };
    Type type;
    Color(Type t) : type(t) { }
    operator Type() const { return type; }
private:
    template<typename T>
    operator T () const;
};

class CdSCell
{
public:
    CdSCell(AnalogInputPin& cds) : cds(cds)
    {

    }

    void WaitForLight();
    Color DetectColor();

private:
    AnalogInputPin& cds;
};

#endif // CDSCELL_H
