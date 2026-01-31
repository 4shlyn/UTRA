#include <Arduino.h>
#include <TCS3200.h>
#include "constants.hpp"

TCS3200 tcs3200(colour::S0, colour::S1, colour::S2, colour::S3, colour::S_Out);
// Create instance of TCS3200 (Colour Sensor) class

String color_indices[] = {"Red", "Green", "Blue"};

RGBColor color_values[] = {
  {255, 0, 0},  // Red
  {0, 255, 0},  // Green
  {0, 0, 255},  // Blue
};

// String clr_read(int h, int s, int v){}

void setup(){

  tcs3200.begin();
  tcs3200.frequency_scaling(TCS3200_OFREQ_20P);
  // Set up sensor - Arduino pins & sensor frequency

  Serial.begin(115200);
  delay(500);

}

void loop(){

  // RGBColor rgb_color = tcs3200.read_rgb_color();

  // Serial.println("Red: " + String(rgb_color.red));
  // Serial.println("Green: " + String(rgb_color.green));
  // Serial.println("Blue: " + String(rgb_color.blue));

  HSVColor hsv = tcs3200.read_hsv();
  
  int hue = hsv.hue;
  int sat = hsv.saturation;
  int val = hsv.value;

  Serial.println("Hue: " + String(hsv.hue));
  Serial.println("Saturation: " + String(hsv.saturation));
  Serial.println("Value: " + String(hsv.value));
  Serial.println("-----------------------------------");

  delay(500);

}