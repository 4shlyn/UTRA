#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <Arduino.h>
#include <cstdint>

// Drivetrain motor pins
namespace drive {
inline uint8_t directionPins[4] = {4, 5, 2, 3};
inline uint8_t speedPins[2] = {10, 9};
}  // namespace drive

// Color sensor pins
namespace color {
inline uint8_t S2_PIN = A5;
inline uint8_t S3_PIN = 7; 
inline uint8_t OUT_PIN = 8;
}

#endif // CONSTANTS_HPP
