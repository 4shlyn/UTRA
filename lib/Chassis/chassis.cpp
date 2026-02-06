#include "chassis.hpp"

void Chassis::moveTank(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0 && rightSpeed >= 0) {
    drivetrain_.forward(leftSpeed, rightSpeed);
  } else if (leftSpeed < 0 && rightSpeed < 0) {
    drivetrain_.backward(-leftSpeed, -rightSpeed);
  } else if (leftSpeed >= 0 && rightSpeed < 0) {
    drivetrain_.right(leftSpeed, -rightSpeed);
  } else {
    drivetrain_.left(-leftSpeed, rightSpeed);
  }
}

ColorName Chassis::readColor() {
  ColorRGB colorReading = colorSensor_.readNormalized();
  return colorSensor_.classify(colorReading);
}

int Chassis::readDistance() {
  unsigned long distance = ultrasonic_.ping_cm(maxDistCm) / sin(ultrasonicAngle * M_PI / 180.0);
  
  // Return max distance if object out of range
  if (distance == 0) {
    distance = maxDistCm;
  }

  return static_cast<int>(distance);
}

void Chassis::followLine(ColorName lineColor, bool followLeft,
                         std::pair<int, int> speeds, bool reverse) {
  auto [inSpeed, outSpeed] = speeds;
  int dir = reverse ? -1 : 1;

  bool onLine = (readColor() == lineColor);

  // If following left side move left if on the line, right if off the line
  // Vice-versa for following right
  // Reversing uses backward instead and flips motor order
  int leftSpeed  = dir * (followLeft == onLine == reverse ? outSpeed : inSpeed);
  int rightSpeed = dir * (followLeft == onLine == reverse ? inSpeed  : outSpeed);
  this->moveTank(leftSpeed, rightSpeed);
}
