// Color sensor stuff for TCS3200

#include "ColorSensor.h"

ColorSensor* ColorSensor::_instance = nullptr;

// setup pins
ColorSensor::ColorSensor(uint8_t s2, uint8_t s3, uint8_t outPin)
  : _s2(s2), _s3(s3), _out(outPin), _pulseCount(0), _calibrated(false) {
  _black[0] = _black[1] = _black[2] = 0;
  _white[0] = _white[1] = _white[2] = 1;
}

// init the pins and setup interrupt
void ColorSensor::begin() {
  pinMode(_s2, OUTPUT);
  pinMode(_s3, OUTPUT);
  pinMode(_out, INPUT);

  _instance = this;
  attachInterrupt(digitalPinToInterrupt(_out), pulseISR, RISING);
}

// ISR for pulse counting
void ColorSensor::pulseISR() {
  if (_instance) 
    _instance->_pulseCount++;
}

// read freq from sensor with given filter settings
unsigned long ColorSensor::readFrequency(uint8_t s2, uint8_t s3) {
  digitalWrite(_s2, s2);
  digitalWrite(_s3, s3);

  _pulseCount = 0;
  delay(5);  // let it settle

  unsigned long start = millis();
  while (millis() - start < 100) {}  // count for 100ms

  return _pulseCount * 10;  // convert to Hz
}

// get raw RGB readings
void ColorSensor::readRaw(unsigned long rgb[3]) {
  rgb[0] = readFrequency(LOW, LOW);     // red
  rgb[1] = readFrequency(HIGH, HIGH);   // green
  rgb[2] = readFrequency(LOW, HIGH);    // blue
}

// map value between black/white calibration points
float ColorSensor::normalize(unsigned long v, unsigned long b, unsigned long w) const {
  if (w <= b) return 0.0f;  // avoid divide by zero
  
  float n = (float)(v - b) / (float)(w - b);
  return constrain(n, 0.0f, 1.0f);
}

ColorRGB ColorSensor::readNormalized() {
  unsigned long raw[3];
  readRaw(raw);

  // normalize each channel using calibration
  return {
    normalize(raw[0], _black[0], _white[0]),
    normalize(raw[1], _black[1], _white[1]),
    normalize(raw[2], _black[2], _white[2])
  };
}

// do calibration via serial
void ColorSensor::calibrateSerial() {
  unsigned long tmp[3];

  Serial.println("\n--- COLOR CALIBRATION ---");
  Serial.println("Place BLACK object, press any key");
  while (!Serial.available()) {}
  Serial.read();
  delay(300);
  readRaw(_black);

  Serial.println("Place WHITE object, press any key");
  while (!Serial.available()) {}
  Serial.read();
  delay(300);
  readRaw(_white);

  _calibrated = true;
  Serial.println("Calibration complete\n");
}

bool ColorSensor::isCalibrated() const {
  return _calibrated;
}

// figure out what color we're looking at
ColorName ColorSensor::classify(const ColorRGB& c) const {
  // check each color - thresholds found by trial and error
  if (c.r > c.g && c.r > c.b && c.r > 0.2) return ColorName::RED;
  if (c.g > c.r && c.g > c.b && c.g > 0.05) return ColorName::GREEN;  // green needs lower threshold
  if (c.b > c.r && c.b > c.g && c.b > 0.2) return ColorName::BLUE;
  if (c.r > 0.35 && c.g > 0.35 && c.b > 0.35) return ColorName::WHITE;
  if (c.r < 0.1 && c.g < 0.1 && c.b < 0.1) return ColorName::BLACK;
  return ColorName::UNKNOWN;
}

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
