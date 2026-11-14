/*
  Door and Activity Check-in System
  Board: Arduino Nano 33 IoT

  Libraries: WiFiNINA, PubSubClient, Adafruit DHT sensor library,
  Adafruit Unified Sensor.

  IMPORTANT: Replace Wi-Fi placeholders and verify MQTT_SERVER.
  Do not commit real credentials.
*/

#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <math.h>
#include <string.h>

// Add the real network details only in your local copy.
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// Set this to the Raspberry Pi's current LAN IP (hostname -I).
const char* MQTT_SERVER = "192.168.137.254";
const uint16_t MQTT_PORT = 1883;

const char* TOPIC_TELEMETRY = "door/system/telemetry";
const char* TOPIC_HEARTBEAT = "door/system/heartbeat";
const char* TOPIC_ALERT = "door/system/alert";
const char* TOPIC_STATUS = "door/system/status";

#define DHT_PIN 9
#define DHT_TYPE DHT22
#define PIR_PIN 3
#define TRIG_PIN 4
#define ECHO_PIN 5
#define BUTTON_PIN 6
#define GREEN_LED 7
#define RED_LED 8
#define BLUE_LED 10
#define YELLOW_LED 11
#define BUZZER_PIN 12  // Controls a suitable external driver only.

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

const unsigned long HEARTBEAT_INTERVAL = 5000;
const unsigned long DHT_INTERVAL = 2500;
const unsigned long DISTANCE_INTERVAL = 1000;
unsigned long lastHeartbeat = 0;
unsigned long lastDhtRead = 0;
unsigned long lastDistanceRead = 0;

bool helpRequested = false;
bool previousButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_MS = 40;

float temperatureC = NAN;
float humidityPct = NAN;
float distanceCm = NAN;
int motionDetected = 0;

void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, pass);
    for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
  }
  Serial.println("Wi-Fi connected");
  Serial.print("Arduino IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    if (WiFi.status() != WL_CONNECTED) connectWiFi();

    Serial.print("Connecting to MQTT...");
    String clientId = "ArduinoNano33-";
    clientId += String((unsigned long)random(0xffff), HEX);

    if (mqtt.connect(clientId.c_str())) {
      Serial.println(" connected");
      mqtt.publish(TOPIC_STATUS, "Arduino Nano 33 IoT connected");
    } else {
      Serial.print(" failed, state=");
      Serial.println(mqtt.state());
      delay(3000);
    }
  }
}

void readPir() {
  motionDetected = digitalRead(PIR_PIN);
  digitalWrite(YELLOW_LED, motionDetected ? HIGH : LOW);
}

void readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) {
    distanceCm = NAN;
    digitalWrite(BLUE_LED, LOW);
  } else {
    distanceCm = (duration * 0.0343f) / 2.0f;
    digitalWrite(BLUE_LED,
      (distanceCm > 0 && distanceCm <= 100) ? HIGH : LOW);
  }
}

void readDht() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    humidityPct = NAN;
    temperatureC = NAN;
    Serial.println("DHT22 reading failed");
  } else {
    humidityPct = h;
    temperatureC = t;
  }
}

void updateOutputs() {
  digitalWrite(RED_LED, helpRequested ? HIGH : LOW);
  digitalWrite(GREEN_LED, helpRequested ? LOW : HIGH);

  // D12 is a control signal for an appropriately rated driver.
  // Do not connect an unknown buzzer directly to an Arduino pin.
  digitalWrite(BUZZER_PIN, helpRequested ? HIGH : LOW);
}

void readButton() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != previousButtonReading) lastDebounceTime = millis();

  if (millis() - lastDebounceTime > DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;
      if (stableButtonState == LOW) {
        helpRequested = !helpRequested;
        updateOutputs();
        const char* eventMessage =
          helpRequested ? "HELP REQUESTED" : "HELP REQUEST CANCELLED";
        if (mqtt.connected()) mqtt.publish(TOPIC_ALERT, eventMessage);
        Serial.println(eventMessage);
      }
    }
  }
  previousButtonReading = reading;
}

void publishTelemetry() {
  char payload[220];
  char tempBuffer[20];
  char humidityBuffer[20];
  char distanceBuffer[20];

  if (isnan(temperatureC)) strcpy(tempBuffer, "null");
  else dtostrf(temperatureC, 0, 1, tempBuffer);

  if (isnan(humidityPct)) strcpy(humidityBuffer, "null");
  else dtostrf(humidityPct, 0, 1, humidityBuffer);

  if (isnan(distanceCm)) strcpy(distanceBuffer, "null");
  else dtostrf(distanceCm, 0, 1, distanceBuffer);

  snprintf(payload, sizeof(payload),
    "{\"temperature\":%s,\"humidity\":%s,\"motion\":%d,"
    "\"distance\":%s,\"help_requested\":%s}",
    tempBuffer, humidityBuffer, motionDetected, distanceBuffer,
    helpRequested ? "true" : "false");

  mqtt.publish(TOPIC_TELEMETRY, payload);
  Serial.print("Telemetry: ");
  Serial.println(payload);
}

void publishHeartbeat() {
  mqtt.publish(TOPIC_HEARTBEAT, "Arduino ONLINE");
  Serial.println("Heartbeat sent");
}

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  dht.begin();
  randomSeed(analogRead(A0));

  Serial.println("Door and Activity Check-in System starting");
  connectWiFi();
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setBufferSize(512);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  unsigned long now = millis();
  readButton();
  readPir();

  if (now - lastDistanceRead >= DISTANCE_INTERVAL) {
    lastDistanceRead = now;
    readDistance();
  }

  if (now - lastDhtRead >= DHT_INTERVAL) {
    lastDhtRead = now;
    readDht();
    publishTelemetry();
  }

  if (now - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    lastHeartbeat = now;
    publishHeartbeat();
  }
}
