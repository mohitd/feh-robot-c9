/**
 * @file drivetrain.h
 * @author Mohit Deshpande
 */

#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <FEHMotor.h>
#include <FEHIO.h>

/**
 * @brief The LRDirection struct for left and right directions for turning.
 * This is really a hacked, safer version of an enum.
 */
struct LRDirection
{
    enum Type
    {
        Left, Right
    };
    Type type;
    LRDirection(Type t) : type(t) { }
    operator Type() const { return type; }
private:
    template<typename T>
    operator T () const;
};

/**
 * @brief The LRDirection struct for forward and backward directions for driving.
 * This is really a hacked, safer version of an enum.
 */
struct FBDirection
{
    enum Type
    {
        Forward, Backward
    };
    Type type;
    FBDirection(Type t) : type(t) { }
    operator Type() const { return type; }
private:
    template<typename T>
    operator T () const;
};

/**
 * @brief The DriveTrain class contains all functionality needed to move, turn, and check RPS.
 * All of the private variables are reference types so they're really safe.
 */
class DriveTrain
{
public:
    /**
     * @brief DriveTrain constructor necessary.
     * @param leftMotor Left motor of the robot (left Igwan)
     * @param rightMotor Right motor of the robot (right Igwan)
     * @param leftEncoder Left encoder of the robot (left Igwan encoder)
     * @param rightEncoder Right encoder of the robot (right Igwan encoder)
     */
    DriveTrain(FEHMotor& leftMotor, FEHMotor& rightMotor, DigitalEncoder& leftEncoder, DigitalEncoder& rightEncoder)
        : leftMotor(leftMotor), rightMotor(rightMotor), leftEncoder(leftEncoder), rightEncoder(rightEncoder)
    {

    }

    /**
     * @brief Drive Move the robot foward or backward at a motor percent for a distance
     * @param direction Either forwards and backwards @see FBDirection
     * @param percent Motor percentage to <b>BOTH</b> motors
     * @param distance The distance to travel in @p direction
     */
    void Drive(FBDirection direction, float percent, float distance);

    /**
     * @brief Drive Move the robot foward or backward at different motor percents for a distance
     * @param direction Either forwards and backwards @see FBDirection
     * @param leftPercent Motor percentage given to the left motor
     * @param rightPercent Motor percentage given to the right motor
     * @param distance The distance to travel in @p direction
     */
    void Drive(FBDirection direction, float leftPercent, float rightPercent, float distance);


    /**
     * @brief Drive Move the robot foward or backward at a motor percent for some number of counts.
     * @param direction Either forwards and backwards @see FBDirection
     * @param percent Motor percentage to <b>BOTH</b> motors
     * @param counts Number of counts to move
     */
    void Drive(FBDirection direction, float percent, int counts);

    /**
     * @brief Drive Move the robot foward or backward at different motor percents for a distance.
     * @param direction Either forwards and backwards @see FBDirection
     * @param leftPercent Motor percentage given to the left motor
     * @param rightPercent Motor percentage given to the right motor
     * @param counts Number of counts to move
     */
    void Drive(FBDirection direction, float leftPercent, float rightPercent, int counts);

    /**
     * @brief Drive Start the motors
     * @param percent Start both motors at both this percent
     */
    void Drive(FBDirection direction, float percent);

    /**
     * @brief Stop Stop both motors
     */
    void Stop(); // Hammertime!

    /**
     * @brief Accelerate Accelerates the robot from one percent to another in a direction for a time.
     * This function linearly interpolates from @p startPercent to @p endPercent across @p time.
     *
     * @param direction Either forwards and backwards @see FBDirection
     * @param startPercent Percent to start at
     * @param endPercent Percent to end at
     * @param time How long to take to accelerate
     */
    void Accelerate(FBDirection direction, float startPercent, float endPercent, float time);

    /**
     * @brief Turn Turns in a left or right direction with a percent for counts
     * @param direction Either left or right @see LRDirection
     * @param percent Motor percentage to <b>BOTH</b> motors
     * @param counts Number of counts to turn
     */
    void Turn(LRDirection direction, float percent, int counts);

    /**
     * @brief Turn Turns exactly 90 degrees in the specified direction at a percent
     * @param direction Either left or right @see LRDirection
     * @param percent Motor percentage to <b>BOTH</b> motors
     */
    void Turn(LRDirection direction, float percent);

    /**
     * @brief CheckX Moves the robot to the specified RPS X coordinate
     * @param x RPS X coodinate to move
     * @param facingPlus @c true if the "front" of the robot is facing in the +X direction
     */
    void CheckX(float x, bool facingPlus);

    /**
     * @brief CheckY Moves the robot to the specified RPS Y coordinate
     * @param y RPS Y coodinate to move
     * @param facingPlus @c true if the "front" of the robot is facing in the +Y direction
     */
    void CheckY(float y, bool facingPlus);

    /**
     * @brief CheckHeading Orients the robot to the specified heading
     * @param heading Rotate the robot to this heading
     * @note This function takes the shortest path to rotate the robot from its current heading to the given one
     */
    void CheckHeading(float heading);

private:
    FEHMotor& leftMotor;            /**< Left motor of the robot (left Igwan) */
    FEHMotor& rightMotor;           /**< Right motor of the robot (right Igwan) */
    DigitalEncoder& leftEncoder;    /**< Left encoder of the robot (left Igwan encoder) */
    DigitalEncoder& rightEncoder;   /**< Right encoder of the robot (right Igwan encoder) */

    /**
     * @brief ResetCounts For convenience, resets counts on @c leftEncoder and @c rightEncoder
     */
    void ResetCounts();

};

#endif // DRIVETRAIN_H
