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
     * @brief Lower Self-explanatory
     * Before the cage is lowered, since the function is dependent on time, Raise() is called first
     */
    void Lower();

    /**
     * @brief Raise Self-explanatory
     */
    void Raise();

    /**
     * @brief RaiseHalf Self-explanatory
     */
    void RaiseHalf();

    /**
     * @brief Lower3_4 Lowers cage 3/4 of the total way
     */
    void Lower3_4();

    /**
     * @brief Lower1_4 Lowers cage 1/4 of the total way
     */
    void Lower1_4();

private:
    FEHMotor& cageMotor;            /**< The motor that controls the cage and crank */
    DigitalInputPin& cage_switch;   /**< The switch corresponding to cage being vertical */
    DigitalInputPin& crank_switch;  /**< The switch corresponding to the crank being oriented in an initial position */
};

#endif // CAGE_H
