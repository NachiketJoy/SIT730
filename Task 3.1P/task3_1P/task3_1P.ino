#include <WiFiNINA.h>
#include <PubSubClient.h> 
#include <BH1750.h>
#include <Wire.h>

// ---------- WiFi Setup----------
char ssid[] = "Wifi name";
char pass[] = "Wifi password";


WiFiClient tsClient;


// ---------- MQTT Setup----------
#define MQTT_SERVER "Server.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_TOPIC "sensor/motion"
#define MQTT_USER "Username"
#define MQTT_PASSWORD "Password"

// SSL client required for HiveMQ Cloud
WiFiSSLClient wifiSSLClient;
PubSubClient mqttClient(wifiSSLClient);


// ---------- Sensors ----------
BH1750 lightMeter;

// ---------- Function Prototypes ----------
void connectWiFi();
void connectMQTT();

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(5, OUTPUT);

  connectWiFi();

  // Initialize I2C + BH1750
  Wire.begin();
  delay(200); 
  
if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23)) {
  Serial.println("Error initializing BH1750!");
  delay(2000);
  return;
}

  Serial.println(F("BH1750 Test"));

  // MQTT
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  connectMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  float lux = lightMeter.readLightLevel();

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  // LED indicator
  if (lux >= 3500) {
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(5, LOW);
  }

  // Publish LUX value to MQTT
  char msg[10];
  sprintf(msg, "%.0f", lux);
  mqttClient.publish(MQTT_TOPIC, msg);

  delay(1000);
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

// ---------- MQTT ----------
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");

    if (mqttClient.connect("ArduinoNano33IoT", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT Broker!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}