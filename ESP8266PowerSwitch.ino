#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <RCSwitch.h>

// Transmitter is connected to Arduino Pin #4
#define PIN 4

const char* ssid = "*******";
const char* password = "********";
const char* host = "esp8266PowerSwitch";

RCSwitch mySwitch = RCSwitch();
WiFiClient wclient;
// Server running MQTT
IPAddress MQTTserver(192, 168, 11, 22);
PubSubClient client(MQTTserver, 1883, wclient);

void callback (char* topic, byte* payload, uint8_t length) {
  uint16_t i;

  // handle message arrived
  char* myPayload = (char *) malloc(sizeof(char) * (length + 1));
  strncpy(myPayload, (char *) payload, length);
  myPayload[length] = '\0';
  Serial.print(topic);
  // Serial.print(" (");
  // Serial.print(length);
  // Serial.print(") => ");
  Serial.print(" => ");
  Serial.println(myPayload);

  // Switch 00001
  if (! strcmp(topic, "PowerSwitch1A")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00001", "10000");
      Serial.println("PowerSwitch1A on");
    }
    else {
      mySwitch.switchOff("00001", "10000");
      Serial.println("PowerSwitch1A off");
    }
  }
  else if (! strcmp(topic, "PowerSwitch1B")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00001", "01000");
      Serial.println("PowerSwitch1B on");
    }
    else {
      mySwitch.switchOff("00001", "01000");
      Serial.println("PowerSwitch1B off");
    }
  }
  else if (! strcmp(topic, "PowerSwitch1C")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00001", "00100");
      Serial.println("PowerSwitch1C on");
    }
    else {
      mySwitch.switchOff("00001", "00100");
      Serial.println("PowerSwitch1C off");
    }
  }
  else if (! strcmp(topic, "PowerSwitch1D")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00001", "00001");
      Serial.println("PowerSwitch1D on");
    }
    else {
      mySwitch.switchOff("00001", "00001");
      Serial.println("PowerSwitch1D off");
    }
  }
  // Switch 00010
  else if (! strcmp(topic, "PowerSwitch2A")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00010", "10000");
      Serial.println("PowerSwitch2A on");
    }
    else {
      mySwitch.switchOff("00010", "10000");
      Serial.println("PowerSwitch2A off");
    }
  }
  else if (! strcmp(topic, "PowerSwitch2B")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00010", "01000");
      Serial.println("PowerSwitch2B on");
    }
    else {
      mySwitch.switchOff("00010", "01000");
      Serial.println("PowerSwitch2B off");
    }
  }
  else if (! strcmp(topic, "PowerSwitch2C")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00010", "00100");
      Serial.println("PowerSwitch2C on");
    }
    else {
      mySwitch.switchOff("00010", "00100");
      Serial.println("PowerSwitch2C off");
    }
  }
  else if (! strcmp(topic, "PowerSwitch2D")) {
    if (! strncmp((char*) payload, "on", length)) {
      mySwitch.switchOn("00010", "00010");
      Serial.println("PowerSwitch2D on");
    }
    else {
      mySwitch.switchOff("00010", "00010");
      Serial.println("PowerSwitch2D off");
    }
  }
}

void setup() {
  /*
   * serial
   */
  Serial.begin(76800);
  Serial.println("Booting");

  /*
   * rcswitch
   */
  mySwitch.enableTransmit(PIN);
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(15);

  /*
   * wifi
   */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(host);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
      Serial.println("Start");
      });

  ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      });

  ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // MQTT callback
  client.setCallback(callback);
}

void loop() {

  // handle OTA updates
  ArduinoOTA.handle();

  // handle client connections
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("ESP8266: PowerSwitch")) {
        client.publish("outTopic", "hello world");
        // Switch 00001
        client.subscribe("PowerSwitch1A");
        Serial.println("Subscribe 1A");
        client.subscribe("PowerSwitch1B");
        Serial.println("Subscribe 1B");
        client.subscribe("PowerSwitch1C");
        Serial.println("Subscribe 1C");
        client.subscribe("PowerSwitch1D");
        Serial.println("Subscribe 1D");
        // Switch 00010
        client.subscribe("PowerSwitch2A");
        Serial.println("Subscribe 2A");
        client.subscribe("PowerSwitch2B");
        Serial.println("Subscribe 2B");
        client.subscribe("PowerSwitch2C");
        Serial.println("Subscribe 2C");
        client.subscribe("PowerSwitch2D");
        Serial.println("Subscribe 2D");
      }
    }
    if (client.connected()) client.loop();
  }
}
