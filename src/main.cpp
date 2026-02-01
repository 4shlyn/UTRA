#include <Arduino.h>
#include "constants.hpp"

void setup() {
  // put your setup code here, to run once:
  pinMode(ir::OUT_1, INPUT);
  pinMode(ir::OUT_2, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  delay(250);
  int val1 = digitalRead(ir::OUT_1);
  int val2 = digitalRead(ir::OUT_2);
  Serial.println("IR1: " + String(val1) + " IR2: " + String(val2));
}
