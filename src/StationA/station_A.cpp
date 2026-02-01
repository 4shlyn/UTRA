#include <Arduino.h>
#include "Station_A.h"
#include "movements.h"

void runStationA() {
  //moveForward(500);
  //Serial.println("Move forward done");
  turnLeft(1000);
  Serial.println("Turn left done");
}
