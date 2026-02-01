/**
 * @file main.cpp
 * @brief Main controller for robot navigation and servo control
 */

#include <Arduino.h>
#include <L298NX2.h>
#include "constants.hpp"
#include <Servo.h>
#include "movements.h"
#include "ultrasonic.h"
#include "StationA/Station_A.h"
#include "StationB/Station_B.h"
#include "robot_mode.h"
#include "mode_select.h"


Servo myServo;
constexpr int SERVO_PIN = 6;
int pos = 0; // servo pos

/**
 * @brief Initialize serial communication and attach servo motor
 */
void setup()
{
  Serial.begin(9600);

  unsigned long start = millis();
  while (!Serial && millis() - start < 2000) {}

  Serial.println("Testing (pls work)");
  myServo.attach(SERVO_PIN);

}

/**
 * @brief Main control loop - checks distance and executes movement pattern
 */
void loop()
{
  // put your main code here, to run repeatedly:

  Serial.println(checkDistance());
}
void setup() {
  Serial.begin(9600);
  initModePins();

  delay(2000);  // placement grace period

  RobotMode mode = readRobotMode();

  Serial.print("MODE: ");
  Serial.println(static_cast<int>(mode));

  switch (mode) {
    case RobotMode::STATION_A:
      runStationA();
      break;

    case RobotMode::STATION_B:
      runStationB();
      break;

    // case RobotMode::FINAL_RUN:
    //   runFinalRun();
    //   break;

    // case RobotMode::TEST:
    //   runTestMode();
    //   break;

    default:
      Serial.println("IDLE");
      break;
  }
}
