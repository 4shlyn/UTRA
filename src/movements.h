#ifndef MOVEMENTS_H
#define MOVEMENTS_H

/**
 * @file movements.h
 * @brief Movement control functions for robot drivetrain and servo
 */

#include <Arduino.h>
#include <Servo.h>
#include <L298NX2.h>
extern L298NX2 drivetrain;

/**
 * @brief Drive the robot forward for specified time
 */
void moveForward(int duration);

/**
 * @brief Rotate robot left in place
 */
void turnLeft(int duration);

/**
 * @brief Rotate robot right in place
 */
void turnRight(int duration);

/**
 * @brief Raise the servo arm to pickup position
 * @param myServo Reference to servo object
 */
void elevateBox(Servo &myServo);

/**
 * @brief Lower the servo arm to drop position
 * @param myServo Reference to servo object
 */
void lowerBox(Servo &myServo);


#endif