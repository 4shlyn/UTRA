#include "stations.hpp"

void runStationC() {
  // Straight send down ramp until blue tape
  while (chassis.readColor() != ColorName::BLUE) {
    chassis.moveTank(255, 255);
    delay(50);
  }
  delay(250);
  // Track green line until intersection
  while (chassis.readColor() != ColorName::RED) {
    chassis.followLine(ColorName::GREEN, false);
    delay(50);
  }

  // Turn towards reupload zone and prepare to pick up box
  arm.lower();
  bool wasWeavingRight = (chassis.getBufferedColor() == ColorName::GREEN);
  chassis.moveTank(255, -255, wasWeavingRight ? 1000 : 1750, false);
  chassis.moveTank(255, 255);

  // Keep going until hitting detection zone, or reset if needed
  Serial.println("Station C complete");
}
