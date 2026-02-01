#ifndef ULTRASONIC_H
#define ULTRASONIC_H

/**
 * @file ultrasonic.h
 * @brief Ultrasonic distance sensor wrapper using NewPing library
 */

#include <Arduino.h>
#include <NewPing.h>
#include <constants.hpp>

NewPing ultrasonic(sonar::TRIG, sonar::ECHO);

/**
 * @brief Get current distance reading from ultrasonic sensor
 * @return Distance in centimeters
 */
int checkDistance () {
    return ultrasonic.ping_cm();
}

#endif