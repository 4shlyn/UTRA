#include <Arduino.h>
#include "devices.hpp"
#include "stations.hpp"

Station currentStation = Station::STATION_A;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial && millis() < 2000) {}
  
  Serial.println("Initializing…");
  drivetrain.setPinModes();
  chassis.stop();
  arm.raise();
  delay(3000);
  Serial.println("Initialization complete");

  switch (currentStation) {
    case Station::STATION_A:
      runStationA();
      break;
    case Station::STATION_B:
      runStationB();
      break;
    case Station::STATION_C:
      runStationC();
      break;
    case Station::STATION_D:
      runStationD();
      break;
    default:
      Serial.println("No station selected or station not implemented.");
      break;
  }
}

void loop() {
}
