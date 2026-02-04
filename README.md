# UTRA Robotics Control System

A modular robot control system built with Arduino framework and PlatformIO for autonomous navigation and task execution across multiple stations.

## Overview

This project provides a software control hub for a mobile robot platform designed to execute various station-based tasks. The architecture emphasizes clean separation of concerns, software-driven configuration, and deterministic behavior execution.

## Hardware Requirements

- **Microcontroller**: Arduino UNO R4 Minima
- **Motor Driver**: L298N dual H-bridge motor controller
- **Sensors**:
  - Ultrasonic distance sensor (HC-SR04 or compatible)
  - Color sensor (optional, library included)
- **Actuators**:
  - Servo motor
  - DC motors for drivetrain

## Pin Configuration

### Motor Driver (L298N)
- EN_A: Pin 9
- IN1_A: Pin 2
- IN2_A: Pin 3
- EN_B: Pin 10
- IN1_B: Pin 4
- IN2_B: Pin 5

### Sensors
- Ultrasonic TRIG: Pin 11
- Ultrasonic ECHO: Pin 12

## Project Structure

```
UTRA-main/
├── src/
│   ├── main.cpp              # Main program dispatcher
│   ├── movements.cpp/h       # Robot movement control
│   ├── robot_mode.h          # Mode enumeration
│   ├── ultrasonic.h          # Ultrasonic sensor wrapper
│   ├── StationA/             # Station A task logic
│   ├── StationB/             # Station B task logic
│   ├── StationC/             # Station C task logic
│   └── StationTest/          # Testing routines
├── lib/                      # Custom color sensor library
├── include/
│   └── constants.hpp         # Hardware pin definitions
└── platformio.ini            # Build configuration
```

## Dependencies

The following libraries are automatically managed by PlatformIO:
- `L298N` (v2.0.3) - Motor driver control
- `Servo` (v1.1.8) - Servo motor control
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
2. Modify the `START_MODE` constant:
   ```cpp
   constexpr RobotMode START_MODE = RobotMode::STATION_A;
   ```
3. Available modes:
   - `RobotMode::STATION_A`
   - `RobotMode::STATION_B`
   - `RobotMode::STATION_C`
   - `RobotMode::TEST`
   - `RobotMode::IDLE`
4. Re-upload the code to the robot

No other code changes are required.

### Implementing New Station Logic

Each station has its own subdirectory with implementation and header files:

```cpp
// Example: StationA/Station_A.h
#pragma once
void runStationA();

// Example: StationA/station_A.cpp
#include "Station_A.h"
#include "movements.h"

void runStationA() {
  moveForward(1000);
  turnLeft(500);
  // Add your station logic here
}
```

## Available Movement Functions

```cpp
void moveForward(int duration);    // Drive forward (ms)
void turnLeft(int duration);       // Rotate left (ms)
void turnRight(int duration);      // Rotate right (ms)
void raiseArm();                   // Raise servo arm
void lowerArm();                   // Lower servo arm
int checkDistance();               // Get ultrasonic reading (cm)
```

## Design Philosophy

- **Software-Driven Configuration**: Change behavior by modifying a single constant
- **Modular Architecture**: Each station is self-contained and isolated
- **Deterministic Execution**: All logic runs once in `setup()`, not in `loop()`
- **Clean Separation**: Hardware pins centralized in `constants.hpp`

## Development

### Adding a New Station

1. Create a new directory: `src/StationD/`
2. Add header file: `Station_D.h` with function declaration
3. Add implementation: `station_D.cpp` with logic
4. Add enum value to `robot_mode.h`: `STATION_D`
5. Update switch statement in `main.cpp`

### Debugging

The robot waits 2 seconds after `setup()` begins, allowing time for:
- Serial connection establishment
- Physical placement and preparation

Monitor serial output at 115200 baud for debugging information.

## License

This project is a winning submission of the UTRA (University of Toronto Robotics Association) 2026 hackathon.
