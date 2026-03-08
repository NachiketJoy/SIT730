# Task 1.1 - Switching ON Lights

## Project Overview
This project simulates a simple home lighting automation system for Linda.
When she arrives home and presses a button, two lights turn on:

- The porch light LED turns on for 30 seconds
- The hallway light LED turns on for 60 seconds

Each light stays on for a set amount of time and then turns off automatically.

In this program:
- The porch light is connected to pin `8`
- The hallway light is connected to pin `6`
- The push button is connected to pin `10`

## Aim
To make it safer and easier for Linda to enter her house at night

## How the System Works
When the button is pressed, the program records the current time and switches both LEDs on.

Each light has its own timer:
- The porch LED turns off after 30 seconds
- The hallway LED turns off after 60 seconds

The button does not toggle the lights.
It only starts the timers once per press.

## Components Used
- Arduino Nano 33 IoT
- 1 push button
- 2 LEDs
- Resistors (2 * 220 ohms)
- Breadboard
- Jumper wires

## Code Structure

### `setup()`
This function defines the porch LED pin and hallway LED pin as outputs and configures the button pin as `INPUT_PULLUP`.

### `loop()`
Detects the button press and calls the update functions continuously.

### `triggerLights()`
Starts both timers and turns both LEDs on.

### `updatePorchLight()`
Checks if 30 seconds have passed and turns the porch LED off

### `updateHallLight()`
Checks if 60 seconds have passed and turns the hallway LED off

## Circuit Schematic
- [alt text](Light_on.png)


## Demonstration Video
Link:

## Modular Programming
Modular programming helped keep the logic organized.

Each part of the behavior (button press, porch timer, hallway timer) is handled by its own function.

This made it easier to test each part separately and adjust timing without touching the rest of the code.

If more lights or sensors are added later, new modules can be added without changing the core structure.

## Design Issues and Limitations

- The system relies on a button press. It does not detect when Linda actually arrives.
- There is no light sensor, so the lights turn on even during the day.

## Future Modules for Smart Assisted Living
### 1. Arrival Detection Module
Lights turn on automatically when Linda approaches the house using a small tag on her. This would remove the need for Linda to press a button.

### 2. Routine‑Based Automation Module
This module will learn Linda’s usual arrival times and adjusts lighting automatically.

### 3. Progressive Pathway Lighting Module
Add more lights and this module would turn lights on as Linda walks forward and turn lights off behind her automatically

## ARM‑Based Embedded System
Modern cars include an Advanced Driver Assistance System (ADAS) that helps the driver stay safe on the road.

The ADAS controller is an embedded system built around an ARM‑based processor as they are efficient and suitable for real‑time decision‑making.

### How the ARM Processor Is Used
The ARM processor inside the ADAS controller processes data from multiple sensors and supports features such as:
- Lane‑keeping assistance
- Adaptive cruise control
- Automatic emergency braking

### Additional Components Connected to the ADAS Controller
- Cameras
Front‑facing and side cameras provide visual information for lane detection, object recognition, and traffic signs.

- Radar Sensors
Used to measure distance and speed of vehicles ahead, especially for adaptive cruise control and collision avoidance.

### Recent Methods and Frameworks Used in Embedded System Design
- Model‑Based Development (MBD)
Tools like MATLAB/Simulink allow engineers to design ADAS algorithms visually.
The tool can generate C/C++ code automatically, which reduces development time and errors.
- AUTOSAR (Automotive Open System Architecture)
Standardises how software components communicate inside the car.
This makes ADAS software easier to update, test, and reuse across different vehicle models
