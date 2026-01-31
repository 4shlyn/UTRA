#include <Arduino.h>
#include <Servo.h>
#include "movements.h"

Servo myServo;

constexpr int SERVO_PIN = 6;
int pos = 0; // servo pos

void setup()
{
  myServo.attach(SERVO_PIN);
}

void loop()
{  
  elevateBox();
  delay(1000); 
  lowerBox();
  delay(1000); 
  
}