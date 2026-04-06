#include <WiFiNINA.h>
#include <WiFiServer.h>

// ---------------- LED Pins ----------------
#define LED_LIVING 6
#define LED_BATHROOM 7
#define LED_CLOSET 8

// ---------------- WiFi ----------------
char ssid[] = "";
char pass[] = "";

WiFiServer server(80);

// ---------------- URL decode ----------------
String decode(String s) {
  s.replace("%20", " "); // convert it to normal text, to match room names
  return s;
}

// ---------------- Extract parameter ----------------
String getParam(String req, String key) {
  // Isolates the room, state, and time
  int start = req.indexOf(key + "=");
  if (start == -1) return "";
  start += key.length() + 1;

  int end = req.indexOf("&", start);
  if (end == -1) end = req.indexOf(" ", start);
  if (end == -1) end = req.length();

  return decode(req.substring(start, end));
}

// ---------------- Map room → pin ----------------
int getPin(String room) {
  if (room == "living room") return LED_LIVING;
  if (room == "bathroom") return LED_BATHROOM;
  if (room == "closet") return LED_CLOSET;
  return -1;
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_LIVING, OUTPUT);
  pinMode(LED_BATHROOM, OUTPUT);
  pinMode(LED_CLOSET, OUTPUT);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// ---------------- Loop ----------------
void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  client.flush();
  Serial.println(req);

  // ----------- TOGGLE -----------
  if (req.indexOf("/toggle?") >= 0) {
    String room  = getParam(req, "room");
    String state = getParam(req, "state");

    int pin = getPin(room);

    if (pin != -1) {
      digitalWrite(pin, state == "on" ? HIGH : LOW);
      Serial.println(room + " -> " + state);
    }
  }

  // ----------- TIMER -----------
  if (req.indexOf("/timer?") >= 0) {
    String room = getParam(req, "room");
    int seconds = getParam(req, "time").toInt();

    int pin = getPin(room);

    if (pin != -1 && seconds > 0) {
      Serial.println(room + " timer " + String(seconds) + "s");
      digitalWrite(pin, HIGH);
      delay(seconds * 1000);
      digitalWrite(pin, LOW);
      Serial.println(room + " timer done");
    }
  }

  // ----------- RESPONSE -----------
  client.println("HTTP/1.1 200 OK");
  client.println("Connection: close");
  client.println();
  client.stop();
}