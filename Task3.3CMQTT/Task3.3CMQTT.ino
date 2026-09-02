#include <WiFiNINA.h>
#include <PubSubClient.h>

// Wi-Fi
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";


// EMQX MQTT broker
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;

// Pin connections
const int trigPin = 9;
const int echoPin = 10;
const int hallwayLED = 5;
const int bathroomLED = 6;

WiFiClient wifi;
PubSubClient mqtt(wifi);

// Used to detect how long the hand stays near the sensor
bool handNear = false;
unsigned long handStart;

void setup() {
  Serial.begin(9600);

  // Set sensor and LED pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(hallwayLED, OUTPUT);
  pinMode(bathroomLED, OUTPUT);

  // Connect to Wi-Fi
  connectWiFi();

  // Set up MQTT connection
  mqtt.setServer(mqttServer, mqttPort);
  mqtt.setCallback(callback);
  connectMQTT();
}

void loop() {

  // Reconnect to MQTT if connection is lost
  if (!mqtt.connected())
    connectMQTT();

  // Check for incoming MQTT messages
  mqtt.loop();

  // Get the distance measured by the ultrasonic sensor
  float distance = getDistance();

  // Check if a hand is within 15 cm
  if (distance > 0 && distance < 15) {

    if (!handNear) {
      handNear = true;
      handStart = millis();
    }

  } else {

    // Hand has moved away from the sensor
    if (handNear) {

      unsigned long duration = millis() - handStart;

      // Short movement = Wave
      if (duration < 700) {
        Serial.println("WAVE detected");
        mqtt.publish("ES/Wave", "Vasu");
      }

      // Longer movement = Pat
      else {
        Serial.println("PAT detected");
        mqtt.publish("ES/Pat", "Vasu");
      }

      handNear = false;
      delay(500);
    }
  }

  delay(50);
}

// ---------------- Wi-Fi ----------------

void connectWiFi() {

  Serial.print("Connecting to Wi-Fi");

  // Keep trying until Wi-Fi is connected
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("\nWi-Fi connected!");
}

// ---------------- MQTT ----------------

void connectMQTT() {

  // Keep trying until connected to EMQX
  while (!mqtt.connected()) {

    Serial.print("Connecting to EMQX...");

    // Create a unique MQTT client ID
    String id = "Nano33IoT-";
    id += String(random(10000));

    if (mqtt.connect(id.c_str())) {

      Serial.println("connected!");

      // Subscribe to both assignment topics
      mqtt.subscribe("ES/Wave");
      mqtt.subscribe("ES/Pat");

    } else {

      Serial.println(" failed");
      delay(5000);
    }
  }
}

// ---------------- MQTT messages ----------------

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Received: ");

  // Display the received message
  for (int i = 0; i < length; i++)
    Serial.print((char)payload[i]);

  Serial.print(" | Topic: ");
  Serial.println(topic);

  // Wave message turns both lights ON
  if (strcmp(topic, "ES/Wave") == 0) {
    digitalWrite(hallwayLED, HIGH);
    digitalWrite(bathroomLED, HIGH);
  }

  // Pat message turns both lights OFF
  if (strcmp(topic, "ES/Pat") == 0) {
    digitalWrite(hallwayLED, LOW);
    digitalWrite(bathroomLED, LOW);
  }
}

// ---------------- Ultrasonic ----------------

float getDistance() {

  // Send a short ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the returning echo
  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
    return -1;

  // Convert echo time into distance in centimetres
  return duration * 0.0343 / 2;
}
