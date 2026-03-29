#include <WiFiNINA.h>
#include <PubSubClient.h> 
#include <BH1750.h>
#include <Wire.h>

// ---------- WiFi Setup----------
char ssid[] = "Optus_B628_353E";
char pass[] = "3Mqaq5RELDB";


// char appPassword= "vsdj kcia avtf fghz"

WiFiClient tsClient;

// ---------- ThingSpeak ----------
unsigned long channelID = 3291392;
const char * writeAPIKey = "QRJF38XPVO4RLV90";

// ---------- MQTT Setup----------
#define MQTT_SERVER "075a2ed74b2d4cfda5a5afe6d7859a25.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_TOPIC "sensor/motion"
#define MQTT_USER "Ovi10"
#define MQTT_PASSWORD "Lbje9U.xE@v8Xvn"

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

  // Connect WiFi
  connectWiFi();

  // Initialize I2C + BH1750
  Wire.begin();
  delay(200); 
  
if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire)) {
  Serial.println("Error initializing BH1750!");
  delay(2000);
  return; // don't freeze board
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


// Test code
// #include <Wire.h>

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
//   Serial.println("Scanning...");
// }

// void loop() {
//   byte error, address;
//   int count = 0;

//   for (address = 1; address < 127; address++) {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();

//     if (error == 0) {
//       Serial.print("I2C device found at 0x");
//       Serial.println(address, HEX);
//       count++;
//     }
//   }

//   if (count == 0) Serial.println("No I2C devices found");
//   delay(2000);
// }


// Test code for scanning I2C devices on the bus
// void setup() {
//   Serial.begin(9600);
//   while (!Serial); // important for Nano 33 IoT
//   Wire.begin();
// }

// void loop() {
//   Serial.println("Scanning...");

//   byte count = 0;

//   for (byte address = 1; address < 127; address++) {
//     Wire.beginTransmission(address);
//     if (Wire.endTransmission() == 0) {
//       Serial.print("Found device at 0x");
//       Serial.println(address, HEX);
//       count++;
//     }
//   }

//   if (count == 0) {
//     Serial.println("No I2C devices found");
//   }

//   delay(3000); // scan every 3 seconds
// }
