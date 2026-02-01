/**
 * @file ColorSensor.h
 * @brief TCS3200 color sensor driver interface
 */

#pragma once
#include <Arduino.h>

/**
 * @brief Normalized RGB color values
 */
struct ColorRGB {
  float r;
  float g;
  float b;
};

/**
 * @brief Color classification categories
 */
enum class ColorName {
  RED,
  GREEN,
  BLUE,
  WHITE,
  BLACK,
  UNKNOWN
};

/**
 * @brief Convert ColorName to string representation
 * @param c Color name to convert
 * @return String representation
 */
const char* toString(ColorName c);

/**
 * @brief TCS3200 color sensor driver class
 */
class ColorSensor {
public:
  /**
   * @brief Construct a new Color Sensor object
   * @param s2 S2 frequency selection pin
   * @param s3 S3 frequency selection pin
   * @param outPin Output frequency pin
   */
  ColorSensor(uint8_t s2, uint8_t s3, uint8_t outPin);

  /**
   * @brief Initialize sensor pins and interrupts
   */
  void begin();

  /**
   * @brief Perform interactive calibration via Serial
   */
  void calibrateSerial();
  
  /**
   * @brief Check calibration status
   * @return true if calibrated
   */
  bool isCalibrated() const;

  /**
   * @brief Read raw RGB frequency values
   * @param rgb Output array for R, G, B frequencies
   */
  void readRaw(unsigned long rgb[3]);
  
  /**
   * @brief Read normalized RGB values (0.0 to 1.0)
   * @return ColorRGB struct with normalized values
   */
  ColorRGB readNormalized();

  /**
   * @brief Classify RGB reading into a color name
   * @param rgb Normalized RGB values to classify
   * @return ColorName enum
   */
  ColorName classify(const ColorRGB& rgb) const;

private:
  uint8_t _s2, _s3, _out;
  volatile unsigned long _pulseCount;

  unsigned long _black[3];
  unsigned long _white[3];
  bool _calibrated;

  static ColorSensor* _instance;
  static void pulseISR();

  unsigned long readFrequency(uint8_t s2, uint8_t s3);
  float normalize(unsigned long v, unsigned long b, unsigned long w) const;
};
