#include <WiFiNINA.h>
#include <PubSubClient.h>

// ---------- WiFi Setup----------
char ssid[] = "Wifi_NAme";
char pass[] = "Wifi_Password";

// ---------- MQTT ----------
#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883


WiFiClient tsClient;
PubSubClient mqttClient(tsClient);


// ---------- Sensors ----------
const int trigPin = 9;
const int echoPin = 10;
const int ledHallway = 4;
const int ledBathroom = 5;

// ---------- Variables ----------
long duration;
int distance;
unsigned long patStart = 0;
int waveCount = 0;

// ---------- Function Prototypes ----------
void connectWiFi();
void connectMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);

// ---------- Distance Functions ----------
int getDistance() {
  long duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return -1;  // no echo

  int dist = duration * 0.034 / 2;
  if (dist < 2 || dist > 200) return -1;
  return dist;
}


// ---------- Setup ----------
void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledHallway, OUTPUT);
  pinMode(ledBathroom, OUTPUT);

  connectWiFi();

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  connectMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  int distance = getDistance();
  // Serial.print("Distance: ");
  // Serial.println(distance);

  if (distance == -1) {
    delay(100);
    return;
  }

  // Gesture detection
  if (distance <= 10) {
    mqttClient.publish("ES/Pat", "Nachiket Pat");
    Serial.println("PAT detected → Lights OFF");
    delay(1000);
  } else if (distance > 10 && distance <= 30) {
    mqttClient.publish("ES/Wave", "Nachiket Wave");
    waveCount++;

    Serial.print("Wave count: ");
    Serial.println(waveCount);

    if (waveCount >= 3) {
      mqttClient.publish("ES/Alert", "Carer Alert: 3 waves detected");
      Serial.println("ALERT SENT");
      waveCount = 0;
    }
    delay(1000);
  }

  delay(100);
}

// ---------- MQTT CALLBACK ----------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(msg);

  // Subscribe to Wave - Turn ON
  if (String(topic) == "ES/Wave") {
    digitalWrite(ledHallway, HIGH);
    digitalWrite(ledBathroom, HIGH);
    Serial.println("Lights ON");
  }
  // Subscribe to Pat - Turn OFF
  else if (String(topic) == "ES/Pat") {
    digitalWrite(ledHallway, LOW);
    digitalWrite(ledBathroom, LOW);
    Serial.println("Lights OFF");
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

// ---------- MQTT ----------
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("Nano33IoT-Nachiket")) {
      Serial.println("connected");
      mqttClient.subscribe("ES/Wave");
      mqttClient.subscribe("ES/Pat");
    } else {
      delay(5000);
    }
  }
}