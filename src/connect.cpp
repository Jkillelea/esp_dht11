#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#include "conf.h"

// declared in main.ino
extern MQTTClient client;
extern WiFiClient net;

// Tries to connect to WiFi and MQTT server
// if connecting to WiFi times out, the chip will reset in an attempt to
// connect to wifi again
void connect() {
  long unsigned int start_milis = millis();

  // wifi connection
  Serial.print("connecting to " + String(ssid));
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);

    if (millis() - start_milis > WIFI_TIMEOUT_TIME) { // reset if we haven't gotten it yet
      Serial.println("\nResetting...");               // this will go back and start over
      ESP.reset();
    }
  }
  Serial.print("\nwfifi connected. IP: ");
  Serial.print(WiFi.localIP().toString());
  Serial.print(" RSSI: ");
  Serial.println(WiFi.RSSI());

  // mqtt server connection
  Serial.print("\nmqtt connecting");
  client.begin(mqttHostName, mqttPortNum, net);
  while (!client.connect(clientID, mqttUserName, mqttPassword)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nmqtt connected.");
}
