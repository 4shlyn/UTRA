# Vertically Inclined: UTRA Hacks Closed Challenge Submission

A modular robot control system built with Arduino framework and PlatformIO for autonomous navigation and task execution across multiple stations.

Awarded 2nd Place Overall and 1st for Top Track Performance.

## Overview

This project provides a software control hub for a mobile robot platform designed to execute various station-based tasks. The architecture emphasizes clean separation of concerns, software-driven configuration, and deterministic behavior execution.

## Hardware

- **Microcontroller**: Arduino UNO R4 Minima
- **Motor Driver**: L298N dual H-bridge motor controller
- **Sensors**:
  - Ultrasonic distance sensor (HC-SR04)
  - Color sensor (TC3200)
- **Actuators**:
  - Servo motor
  - DC motors

## Pin Configuration

### Motor Driver (L298N)
- EN_A:  9
- IN1_A: 2
- IN2_A: 3
- EN_B:  10
- IN1_B: 4
- IN2_B: 5

### Sensors
- Ultrasonic TRIG: 11
- Ultrasonic ECHO: 12

## Project Structure

```
UTRA-main/
├── include                 // Non-library header files
│   ├── constants.hpp           // Pins and other constants
│   ├── devices.hpp             // Device constructors
│   ├── README
│   └── stations.hpp
├── lib                     // Custom libraries
│   ├── Arm                     // Servo arm library
│   │   ├── arm.cpp
│   │   └── arm.hpp
│   ├── Chassis                 // Chassis library
│   │   ├── chassis.cpp
│   │   └── chassis.hpp
│   ├── ColorSensor             // Color sensor library
│   │   ├── ColorSensor.cpp
│   │   └── ColorSensor.h
│   └── README
├── platformio.ini              // External dependencies
├── README.md
├── src                     // Source code
│   ├── build
│   │   └── Debug
│   ├── main.cpp                // main.cpp
│   └── stations                // Station autonomous routines
│       ├── station_a.cpp
│       ├── station_b.cpp
│       ├── station_c.cpp
│       └── station_d.cpp
└── test
    └── README
```

## Dependencies

The following libraries are automatically managed by PlatformIO:
- `L298N` (v2.0.3) - Motor driver control
- `DDBot` (v1.1.3) - Differential drive control (wrapped by custom chassis class)
- `Servo` (v1.3.0) - Servo motor control (wrapped by custom arm class)
- `NewPing` (v1.9.7) - Ultrasonic sensor interface

## Building and Uploading

### Prerequisites
- [PlatformIO](https://platformio.org/) installed (via VSCode extension or CLI)

### Build
```bash
pio run
```

### Upload to Robot
```bash
pio run --target upload
```

### Monitor Serial Output
```bash
pio device monitor
```

## Usage

### Selecting a Station

To change which station the robot executes:

1. Open `src/main.cpp`
2. Modify the `currentStation` constant:
   ```cpp
   constexpr Station currentStation = Station::STATION_A;
   ```
3. Available stations:
   - `Station::STATION_A`
   - `Station::STATION_B`
   - `Station::STATION_C`
   - `Station::STATION_D`
4. Re-upload the code to the robot

No other code changes are required.

### Implementing New Station Logic

Each station has its own subdirectory with implementation and header files:

```cpp
// Example: stations.hpp
void runStationA();

// Example: stations/station_a.cpp
#include "stations.hpp"

void runStationA() {
  Serial.println("Running Station A");
  // Move to blue tape
  while (chassis.readColor() != ColorName::BLUE) {
    chassis.followLine(ColorName::GREEN, false);
    delay(50);
  }
  // ...
}
```

## Custom Chassis Methods
Full doxygen documentation available for each method
### Movement
```cpp
void moveTank(int leftSpeed, int rightSpeed, int time = 0, bool stopAfter = true);

void stop();

void followLine(ColorName lineColor, bool followLeft, std::pair<int, int> speeds = {170, 255}, bool reverse = false);
```
### Utils
```cpp
ColorName readColor();
ColorName getBufferedColor();

int readDistance();
std::queue<int> getDistanceReadings();
```

## Design Philosophy
- **Software-Driven Configuration**: Change behavior by modifying a single constant
- **Modular Architecture**: Each station is self-contained and isolated
- **Deterministic Execution**: All logic runs once in `setup()`, not in `loop()`
- **Clean Separation**: Hardware pins centralized in `constants.hpp`, device initialization centralized in `devices.hpp`

## Development

### Adding a New Station

1. Forwards declare new method (e.g. `runStationE()`) in `stations.hpp`
2. Create new source file (e.g. `station_e.cpp`) in `src/stations/` with source implementation
4. Add enum value to `Station` in `stations.hpp`: `STATION_E`
5. Update switch statement in `main.cpp`

### Debugging

The robot waits 2 seconds after `setup()` begins, allowing time for:
- Serial connection establishment
- Drivetrain pinmode setting
- Arm resetting

Monitor serial output at 115200 baud for debugging information.

## License

This project is a winning submission of the UTRA (University of Toronto Robotics Association) 2026 hackathon.
