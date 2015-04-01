/**
 * @file cdscell.h
 * @author Mohit Deshpande
 */

#ifndef CDSCELL_H
#define CDSCELL_H

#include <FEHIO.h>
#include <FEHLCD.h>

/**
 * @brief The Color struct for red or blue or no light.
 * This is really a hacked, safer version of an enum.
 */
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

/**
 * @brief The CdSCell class encapsulates the CdS cell
 */
class CdSCell
{
public:
    /**
     * @brief CdSCell constructor necessary
     * @param cds The CdS cell to determine light and color
     */
    CdSCell(AnalogInputPin& cds) : cds(cds)
    {

    }

    /**
     * @brief WaitForLight Pauses the robot until the CdS cell reads light
     */
    void WaitForLight();

    /**
     * @brief DetectColor Uses a filter on the CdS cell to determine the color of light
     * @return The color the CdS cell is registering
     * @see Color
     * @warning Threshold values might need to be altered
     */
    Color DetectColor();

private:
    AnalogInputPin& cds;    /**< The CdS cell to determine light and color */
};

#endif // CDSCELL_H
