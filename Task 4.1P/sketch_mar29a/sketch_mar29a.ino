#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

// --- Pin Definitions ---
const int PIR_PIN = 2;
const int SWITCH_PIN = 3;
const int LED1 = 4;
const int LED2 = 5;

volatile bool pirTriggered = false;
volatile bool switchTriggered = false;
unsigned long lastPrintTime = 0;
unsigned long lastInterruptTime = 0;

// --- PIR Interrupt Service Routine ---
void pirISR() {
  pirTriggered = true;
}


// --- Switch Interrupt Service Routine ---
void switchISR() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 250) {
    switchTriggered = true;
  }
  lastInterruptTime = interruptTime;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // Initialise I2C and light sensor
  Wire.begin();
  lightMeter.begin();

  
  // Attach interrupts for PIR and switch
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchISR, CHANGE);

  Serial.println("--- SYSTEM STARTING ---");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastPrintTime >= 1000) {
    float lux = lightMeter.readLightLevel();
    
    int pirState = digitalRead(PIR_PIN); 
    
    Serial.print("[Monitor] Lux: ");
    Serial.print(lux);
    Serial.print(" | PIR Pin 2 is: ");
    Serial.println(pirState == HIGH ? "ACTIVE (Motion!)" : "IDLE (Quiet)");
    
    lastPrintTime = currentTime;
  }

  // --- Handle PIR Interrupt Event ---
  if (pirTriggered) {
    pirTriggered = false;
    Serial.println(">>> Interrupt: Motion logic triggered!");
    
    float checkLux = lightMeter.readLightLevel();
    // Only toggle lights if room is dark
    if (checkLux < 400) {
      toggleLights();
    } else {
      Serial.println(">>> Logic: Too bright to turn on LEDs.");
    }
  }

  // --- Handle Switch Interrupt Event ---
  if (switchTriggered) {
    switchTriggered = false;
    Serial.println(">>> Interrupt: Switch used!");
    toggleLights();
  }
}

// --- Toggle Both LEDs ---
void toggleLights() {
  bool state = !digitalRead(LED1);
  digitalWrite(LED1, state);
  digitalWrite(LED2, state);
  Serial.print("ACTION: Lights set to ");
  Serial.println(state ? "ON" : "OFF");
}