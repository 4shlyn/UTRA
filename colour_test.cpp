#include <Arduino.h>
#include <TCS3200.h>
#include "constants.hpp"

TCS3200 tcs3200(clr::S0, clr::S1, clr::S2, clr::S3, clr::S_Out);

String color_indices[] = {"Red", "Green", "Blue"};

RGBColor color_values[] = {
  {255, 0, 0},  // Red
  {0, 255, 0},  // Green
  {0, 0, 255},  // Blue
};

void setup(){

    tcs3200.begin();
    tcs3200.frequency_scaling(TCS3200_OFREQ_20P);

    Serial.begin(115200);

    delay(1000);

}

void loop(){

    RGBColor rgb_color = tcs3200.read_rgb_color();

    Serial.println("Red: " + String(rgb_color.red));
    Serial.println("Green: " + String(rgb_color.green));
    Serial.println("Blue: " + String(rgb_color.blue));

    Serial.println("-----------------------------------");

    delay(1000);

}