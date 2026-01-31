#include <Arduino.h>
#include <NewPing.h>
#include "constants.hpp"

NewPing ultrasonic(sonar::TRIG, sonar::ECHO, sonar::max_dist);

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(50);  // Required delay between pings

  // Get distance in centimeters
  unsigned int distance = ultrasonic.ping_cm();

  if (distance == 0) {
    Serial.println("Out of range");
    distance = sonar::max_dist;
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}
