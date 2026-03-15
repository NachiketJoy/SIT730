#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "DHT.h"

// ---------- WiFi ----------
char ssid[] = "";
char pass[] = "";

WiFiClient client;

// ---------- ThingSpeak ----------
unsigned long channelID = 123;
const char * writeAPIKey = "";

// ---------- Sensors ----------
#define DHTPIN 8
#define DHTTYPE DHT11
#define LIGHT_PIN A0


DHT dht(DHTPIN, DHTTYPE);

int alarm = 0;

// ---------- Timing ----------
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 30000;

// ---------- Function Prototypes ----------
void connectWiFi();
float readTemperature();
int readLight();
void sendToThingSpeak(float temp, int light);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(5, OUTPUT);

  connectWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  unsigned long now = millis();

    int lightLevel = readLight();

    if(lightLevel <= 500){
      digitalWrite(5, HIGH);
       alarm = 1;
    } else {
      digitalWrite(5, LOW);
      alarm = 0;
    }

  if (now - lastUpdate >= updateInterval) {
    lastUpdate = now;

    float temperature = readTemperature();
    // int lightLevel = readLight();

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" C | Light: ");
    Serial.println(lightLevel);

    sendToThingSpeak(temperature, lightLevel);
  }
}

// ---------- WiFi ----------
void connectWiFi() {
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(5000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
}

// ---------- Sensor Functions ----------
float readTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("DHT read failed");
    return -999;
  }
  return t;
}

int readLight() {
  return analogRead(LIGHT_PIN);
}

// ---------- ThingSpeak ----------
void sendToThingSpeak(float temp, int light) {
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, light);
  ThingSpeak.setField(3, alarm);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Data sent to ThingSpeak.");
  } else {
    Serial.print("Error sending data: ");
    Serial.println(status);
  }
}

