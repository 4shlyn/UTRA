#include "mode_select.h"
#include <Arduino.h>

// choose pins ONCE
static constexpr int MODE_PIN_0 = 22;
static constexpr int MODE_PIN_1 = 23;

void initModePins() {
  pinMode(MODE_PIN_0, INPUT_PULLUP);
  pinMode(MODE_PIN_1, INPUT_PULLUP);
}

RobotMode readRobotMode() {
  bool b0 = digitalRead(MODE_PIN_0) == LOW;
  bool b1 = digitalRead(MODE_PIN_1) == LOW;

  if (!b0 && !b1) return RobotMode::IDLE;
  if ( b0 && !b1) return RobotMode::STATION_A;
  if (!b0 &&  b1) return RobotMode::STATION_B;
  if ( b0 &&  b1) return RobotMode::FINAL_RUN;

  return RobotMode::IDLE;
}
