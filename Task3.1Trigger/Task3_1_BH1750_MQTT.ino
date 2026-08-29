#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>

char ssid[] = "iphone";
char password[] = "12345678";

const char* mqttServer = "192.168.1.5";
const int mqttPort = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

BH1750 lightMeter;

void connectWiFi() {
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("Arduino IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqttClient.connected()) {

    Serial.print("Connecting to MQTT...");

    String clientID = "ArduinoNano33IoT-";
    clientID += String(random(0xffff), HEX);

    if (mqttClient.connect(clientID.c_str())) {
      Serial.println("connected!");
    } 
    else {
      Serial.print("failed, state=");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);

  Wire.begin();

  if (!lightMeter.begin()) {
    Serial.println("BH1750 not found!");
    while (1);
  }

  Serial.println("BH1750 connected!");

  connectWiFi();

  mqttClient.setServer(mqttServer, mqttPort);
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

  char message[20];

  dtostrf(lux, 1, 2, message);

  mqttClient.publish("terrarium/light", message);

  delay(2000);
}
