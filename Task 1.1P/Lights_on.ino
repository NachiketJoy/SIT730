#define PORCH_LED_PIN 8
#define HALL_LED_PIN 6
#define BUTTON_PIN 10

bool lastButton = HIGH;
unsigned long porchStart = 0;
unsigned long hallStart = 0;
  
bool porchOn = false;
bool hallOn = false;


void setup() {
  pinMode(PORCH_LED_PIN, OUTPUT);
  pinMode(HALL_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading == LOW && lastButton == HIGH) {
    triggerLights();
    delay(200);
  }

  updatePorchLight();
  updateHallLight();

  lastButton = reading;
}

void triggerLights() {
  porchOn = true;
  hallOn = true;

  porchStart = millis();
  hallStart = millis();

  digitalWrite(PORCH_LED_PIN, HIGH);
  digitalWrite(HALL_LED_PIN, HIGH);
}

void updatePorchLight() {
  if (porchOn && millis() - porchStart >= 5000) {
    digitalWrite(PORCH_LED_PIN, LOW);
    porchOn = false;
  }
}

void updateHallLight() {
  if (hallOn && millis() - hallStart >= 8000) {
    digitalWrite(HALL_LED_PIN, LOW);
    hallOn = false;
  }
}


