/**
 * @file ColorSensor.cpp
 * @brief TCS3200 color sensor driver - detects colors using light
 * 
 * This driver lets you read colors from the TCS3200 sensor.
 * The sensor works by measuring how much red, green, and blue light
 * it sees, then figures out what color that is. Usage of this driver is included with doc-type objects. 
 */

#include "ColorSensor.h"

/**
 * This is a special variable that lets Arduino's automatic background
 * system talk to our sensor. Think of it like a bookmark that helps
 * the Arduino find our sensor when it needs to count pulses.
 * 
 * Don't worry about this, it's set up automatically when you call begin().
 */
ColorSensor* ColorSensor::_instance = nullptr;

/**
 * @brief Create a new color sensor
 * @param s2 Arduino pin for color selection (control pin 1)
 * @param s3 Arduino pin for color selection (control pin 2)  
 * @param outPin Arduino pin where sensor sends its readings
 */
ColorSensor::ColorSensor(uint8_t s2, uint8_t s3, uint8_t outPin): 
_s2(s2), _s3(s3), _out(outPin), _pulseCount(0), _calibrated(false) {
  _black[0] = _black[1] = _black[2] = 0;
  _white[0] = _white[1] = _white[2] = 1;
}

/**
 * @brief Start the sensor and set up Arduino pins
 * 
 * Call this once in your setup() function to get the sensor ready.
 * It configures the control pins and prepares the sensor to take readings.
 */
void ColorSensor::begin() {
  pinMode(_s2, OUTPUT);
  pinMode(_s3, OUTPUT);
  pinMode(_out, INPUT);

  _instance = this;
  attachInterrupt(digitalPinToInterrupt(_out), pulseISR, RISING);
}

/**
 * @brief Internal function that counts sensor pulses
 * 
 * The sensor sends pulses (rapid on/off signals) to tell us how much
 * light it sees. This function counts those pulses automatically in the
 * background. You don't need to call this yourself.
 */
void ColorSensor::pulseISR() {
  if (_instance) 
    _instance->_pulseCount++;
}

/**
 * @brief Read one color channel from the sensor
 * @param s2 Control pin setting (LOW or HIGH)
 * @param s3 Control pin setting (LOW or HIGH)
 * @return Number representing how much of that color the sensor sees
 * 
 * Different s2/s3 combinations tell the sensor which color to measure:
 * - s2=LOW, s3=LOW: Measures red light
 * - s2=HIGH, s3=HIGH: Measures green light  
 * - s2=LOW, s3=HIGH: Measures blue light
 */
unsigned long ColorSensor::readFrequency(uint8_t s2, uint8_t s3) {
  digitalWrite(_s2, s2);
  digitalWrite(_s3, s3);

  _pulseCount = 0;
  delay(5); // Wait 5ms for sensor to stabilize. Reduce if the readings are too slow.

  unsigned long start = millis();
  while (millis() - start < 100) {}  // Count pulses for 100ms. Increase for more stable readings, decrease for faster readings.

  return _pulseCount * 10;  // Convert to pulses per second (multiply by 10 because we counted for 100ms)
}

/**
 * @brief Get raw red, green, and blue readings from sensor
 * @param rgb Array to store the three readings: [red, green, blue]
 */
void ColorSensor::readRaw(unsigned long rgb[3]) {
  rgb[0] = readFrequency(LOW, LOW);     // red
  rgb[1] = readFrequency(HIGH, HIGH);   // green
  rgb[2] = readFrequency(LOW, HIGH);    // blue
}

/**
 * @brief Convert a raw sensor reading to a 0-1 scale
 * @param v The raw reading from the sensor
 * @param b The "black" calibration value (darkest reading)
 * @param w The "white" calibration value (brightest reading)
 * @return A number between 0.0 (darkest) and 1.0 (brightest)

 * 
 * This helps make readings consistent even if lighting conditions change.
 */
float ColorSensor::normalize(unsigned long v, unsigned long b, unsigned long w) const {
  if (w <= b) return 0.0f;  // avoid divide by zero
  
  float n = (float)(v - b) / (float)(w - b);
  return constrain(n, 0.0f, 1.0f);
}

/**
 * @brief Get color readings on a 0-1 scale
 * @return RGB values where 0.0 = no color, 1.0 = maximum color
 * 
 * You should calibrate the sensor first for best results.
 * This gives you easier-to-work-with numbers than raw readings.
 */
ColorRGB ColorSensor::readNormalized() {
  unsigned long raw[3];
  readRaw(raw);

  // calibrate each channel so that 0.0 = black and 1.0 = white
  return {
    normalize(raw[0], _black[0], _white[0]),
    normalize(raw[1], _black[1], _white[1]),
    normalize(raw[2], _black[2], _white[2])
  };
}

/**
 * @brief Set up the sensor by showing it dark and bright references
 * 
 * This teaches the sensor what "dark" and "bright" look like:
 * 1. Place a black object under the sensor and press any key
 * 2. Place a white object under the sensor and press any key
 * 
 * After this, the sensor will give more accurate color readings.
 * You need to open the Serial Monitor (Tools > Serial Monitor) to use this.
 */
void ColorSensor::calibrateSerial() {
  unsigned long tmp[3];

  Serial.println("\n--- COLOR CALIBRATION ---");
  Serial.println("Place BLACK object, press any key");
  while (!Serial.available()) {}
  Serial.read();
  delay(300);  // Wait 300ms for hand to move away. Increase if readings are unstable.
  readRaw(_black);

  Serial.println("Place WHITE object, press any key");
  while (!Serial.available()) {}
  Serial.read();
  delay(300);  // Wait 300ms for hand to move away. Increase if readings are unstable.
  readRaw(_white);

  _calibrated = true;
  Serial.println("Calibration complete\n");
}

/**
 * @brief Check if the sensor has been calibrated yet
 * @return true if you've run calibrateSerial(), false otherwise
 */
bool ColorSensor::isCalibrated() const {
  return _calibrated;
}

/**
 * @brief Primary method: determine what color the sensor is looking at
 * @param c The RGB readings from the sensor (values from 0.0 to 1.0)
 * @return The name of the color (RED, GREEN, BLUE, WHITE, BLACK, or UNKNOWN)
 * 
 * This looks at the red, green, and blue values and decides which color
 * best matches. For example, if red is highest, it returns RED.
 * 
 * THRESHOLD VALUES (adjust these if colors aren't detected correctly):
 * - 0.2 = 20% brightness required for red/blue
 * - 0.05 = 5% brightness for green (lower because green sensors are often less sensitive)
 * - 0.35 = 35% on all channels means it's bright enough to be white
 * - 0.1 = 10% on all channels means it's dark enough to be black
 * 
 * HOW TO ADJUST:
 * - If RED isn't detected: Lower the 0.2 threshold (try 0.15)
 * - If too many things look RED: Raise the 0.2 threshold (try 0.25)
 * - If WHITE is detected too easily: Raise 0.35 (try 0.4)
 * - If BLACK is detected too easily: Lower 0.1 (try 0.08)
 */
ColorName ColorSensor::classify(const ColorRGB& c) const {
  // Check each color by comparing the three values
  // For RED: red must be bigger than green AND blue, AND bright enough (>0.2)
  if (c.r > c.g && c.r > c.b && c.r > 0.2) return ColorName::RED;
  
  // For GREEN: green must be biggest, and >0.05 (lower threshold because green is harder to detect)
  if (c.g > c.r && c.g > c.b && c.g > 0.05) return ColorName::GREEN;
  
  // For BLUE: blue must be biggest and bright enough
  if (c.b > c.r && c.b > c.g && c.b > 0.2) return ColorName::BLUE;
  
  // For WHITE: all three colors must be bright (>0.35)
  if (c.r > 0.35 && c.g > 0.35 && c.b > 0.35) return ColorName::WHITE;
  
  // For BLACK: all three colors must be dim (<0.1)
  if (c.r < 0.1 && c.g < 0.1 && c.b < 0.1) return ColorName::BLACK;
  
  // If none of the above match, we don't know what color it is
  return ColorName::UNKNOWN;
}

/**
 * @brief Convert a color to text you can print
 * @param c The color to convert
 * @return Text like "RED", "GREEN", "BLUE", etc.
 * 
 * Useful for printing color names to the Serial Monitor.
 */
const char* toString(ColorName c) {
  switch (c) {
    case ColorName::RED: return "RED";
    case ColorName::GREEN: return "GREEN";
    case ColorName::BLUE: return "BLUE";
    case ColorName::WHITE: return "WHITE";
    case ColorName::BLACK: return "BLACK";
    default: return "UNKNOWN";
  }
}
