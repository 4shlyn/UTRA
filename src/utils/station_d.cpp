#include "stations.hpp"

void runStationD() {
  Serial.println("Running Station D");
  // Arm automatically raises on initialization

  // Move forward until detecting obstacle
  while (chassis.readDistance() > 15) {
    chassis.moveTank(255, 255);
    delay(50);
  }

  // Hardcoded box dropoff
  chassis.moveTank(-255, 255, 750);
  chassis.moveTank(255, 255, 3250);
  arm.lower();

  chassis.stop();
  Serial.println("Station D complete");
}
