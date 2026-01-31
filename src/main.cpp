#include <Arduino.h>
#include <L298NX2.h>
#include "constants.hpp"
#include <Servo.h>
#include "movements.h"

Servo myServo;

constexpr int SERVO_PIN = 6;
int pos = 0; // servo pos
L298NX2 drivetrain(dt::EN_A, dt::IN1_A, dt::IN2_A, dt::EN_B, dt::IN1_B, dt::IN2_B);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Testing (pls work)");
  myServo.attach(SERVO_PIN);
  while (!Serial)
  {
    //do nothing
  }

  drivetrain.setSpeedA(255);
  drivetrain.setSpeedB(255);
}

void loop() {
  // put your main code here, to run repeatedly:
  drivetrain.runA(L298N::FORWARD);
  drivetrain.runB(L298N::FORWARD);
  
  
  elevateBox();
  delay(1000); 
  lowerBox();
  delay(1000); 
}
