/**
 * @file movements.cpp
 * @brief Implementation of robot movement functions
 */

#include "movements.h"
#include "constants.hpp"
#include "Arduino.h"

// define drivetrain ONCE
L298NX2 drivetrain(
  dt::EN_A, dt::IN1_A, dt::IN2_A,
  dt::EN_B, dt::IN1_B, dt::IN2_B
);

void moveForward(int duration)
{
    drivetrain.setSpeedA(1000);
    drivetrain.setSpeedB(1000);
    drivetrain.forwardFor(duration);
}

void turnLeft(int duration)
{
    drivetrain.setSpeedA(-1000);
    drivetrain.setSpeedB(1000);
    drivetrain.forwardFor(duration);

}

void turnRight(int duration)
{
    drivetrain.setSpeedA(1000);
    drivetrain.setSpeedB(-1000);
    drivetrain.forwardFor(duration);
}

void elevateBox(Servo &myServo)
{
  for (int pos = 0; pos <= 40; pos++)
  {
    myServo.write(pos);
    delay(15);
  }
}

void lowerBox(Servo &myServo)
{
  for (int pos = 40; pos >= 0; pos--)
  {
    myServo.write(pos);
    delay(15);
  }
}
