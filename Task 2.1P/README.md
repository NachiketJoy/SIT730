# Task 2.1 - Sending Temperature and Light Levels to the Web

## Project Overview
This project collects temperature and light readings from Linda’s home using an Arduino Nano 33 IoT.

The temperature is measured using a DHT11 sensor, and the light level is measured using an analogue LDR.  
The device connects to WiFi and sends both readings to the ThingSpeak cloud platform every 30 seconds.

An alarm turns on when the light level falls below a set threshold.

## Aim
To allow Linda to remotely view the temperature and light conditions of her home through a simple web interface.

## How the System Works
The Arduino reads two sensors:
- Temperature from the DHT11  
- Light level from the LDR 

Every 30 seconds, the device:
- Reads the latest sensor values  
- Prints them to the serial monitor  
- Sends them to ThingSpeak using the Write API key  

- If the light level is **≤ 500**, the alarm is set to **1**  
- Otherwise, the alarm is **0**  
This value is uploaded as **Field 3**.

## Components Used
- Arduino Nano 33 IoT
- DHT11 temperature sensor 
- 1 LED
- Analogue light sensor (LDR) 
- Resistors (1 * 220 ohms + 1 * 10k ohms)
- Breadboard
- Jumper wires

## Code Structure

### `setup()`
Initialises the DHT sensor, configures the LED pin, connects to WiFi, and starts ThingSpeak.

### `loop()`
Continuously reads the light level and updates the alarm.  
Every 30 seconds, it reads the temperature and sends all values to ThingSpeak.

### `connectWiFi()`
Handles WiFi connection using the onboard WiFi module.

### `readTemperature()`
Reads temperature from the DHT11 and checks for invalid readings.

### `readLight()`
Reads the analogue light level from the LDR.

### `sendToThingSpeak()`
Uploads temperature, light, and alarm values to ThingSpeak using `writeFields()`.

## Demonstration Video
Link: https://drive.google.com/file/d/1mB0R6Rnry_4wn4lyJkiijT397b1Um2ZT/view?usp=sharing 

## Modular Programming
Each task—WiFi connection, sensor reading, alarm logic, and cloud upload—is handled by its own function.  
This keeps the code organised and makes it easy to adjust or extend the system later.

If more lights or sensors are added later, new modules can be added without changing the core structure.

## Design Issues and Limitations
- The alarm currently uses only light level; temperature alarms could be added.  

## Future Modules for Smart Assisted Living
### 1. Temperature‑Based Alarm Module
Triggers alerts when the room becomes too hot or too cold.

### 2. Cloud Notification Module
Uses ThingSpeak webhooks to send SMS or email alerts.