#ifndef DEVICES_HPP
#define DEVICES_HPP

#include "constants.hpp"
#include <DDBot.h>
#include "ColorSensor.h"

inline DDBot drivetrain(drive::directionPins, drive::speedPins);
inline ColorSensor colorSensor(color::S2_PIN, color::S3_PIN, color::OUT_PIN);

#endif  // DEVICES_HPP
