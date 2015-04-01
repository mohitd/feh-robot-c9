/**
 * @file cage.h
 * @author Mohit Deshpande
 */

#ifndef CAGE_H
#define CAGE_H

#include <FEHMotor.h>
#include <FEHIO.h>

/**
 * @brief The Cage class to control the cage mechanism.
 * All of the private variables are reference types so they're really safe.
 */
class Cage
{
public:
    /**
     * @brief Cage constructor necessary
     * @param cageMotor The motor that controls the cage and crank
     * @param cage_switch The switch corresponding to cage being vertical
     * @param crank_switch The switch corresponding to the crank being oriented in an initial position
     */
    Cage(FEHMotor& cageMotor, DigitalInputPin& cage_switch, DigitalInputPin& crank_switch) : cageMotor(cageMotor), cage_switch(cage_switch), crank_switch(crank_switch)
    {

    }

    /**
     * @brief Lower Lowers the cage
     * Before the cage is lowered, since the function is dependent on time, Raise() is called first
     */
    void Lower();

    /**
     * @brief Raise Raises the cage
     */
    void Raise();

    /**
     * @brief RaiseHalf Raises the cage halfway for the crank
     */
    void RaiseHalf();

private:
    FEHMotor& cageMotor;            /**< The motor that controls the cage and crank */
    DigitalInputPin& cage_switch;   /**< The switch corresponding to cage being vertical */
    DigitalInputPin& crank_switch;  /**< The switch corresponding to the crank being oriented in an initial position */
};

#endif // CAGE_H
