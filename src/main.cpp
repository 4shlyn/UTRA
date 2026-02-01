#include <Arduino.h>
#include <L298NX2.h>
#include "constants.hpp"
#include <Servo.h>
#include "movements.h"
#include "ultrasonic.h"
#include "StationA/Station_A.h"
#include "StationB/Station_B.h"
#include "robot_mode.h"

#include <Arduino.h>
#include "robot_mode.h"
// CONFIGURE THIS

constexpr RobotMode START_MODE = RobotMode::STATION_A;

void setup() {
  Serial.begin(115200);
  delay(2000);

  switch (START_MODE) {
    case RobotMode::STATION_A:
      runStationA();
      break;

    //case RobotMode::STATION_B:
      //runStationB();
      //break;

    case RobotMode::FINAL_RUN:
      // runFinalRun();
      break;

    case RobotMode::TEST:
      // runTestMode();
      break;
  }
}

void loop() {}
