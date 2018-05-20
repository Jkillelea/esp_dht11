#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#include "conf.h"

// declared in main.ino
extern MQTTClient client;
extern WiFiClient net;

const static int pause_between_checks = 500; // milliseconds

// Tries to connect to WiFi and MQTT server
// if connecting to WiFi times out, the chip will reset in an attempt to
// connect to wifi again
void connect() {
  long unsigned int start_milis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWiFi not connected");
    Serial.print("connecting to " + String(WIFI_SSID));
  
    // wifi connection
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(pause_between_checks);

      if (millis() - start_milis > WIFI_TIMEOUT_TIME) { // reset if we haven't gotten it yet
        Serial.println("\nResetting...");               // this will go back and start over
        ESP.restart();
      }
    }
    Serial.print("\nWiFi connected. IP: ");
    Serial.print(WiFi.localIP().toString());
    Serial.print(" RSSI: ");
    Serial.println(WiFi.RSSI());
  }


  // mqtt server connection
  client.begin(mqttHostName, mqttPortNum, net);
  if (!client.connected()) {
    Serial.println("\nMQTT not connected");

#if CONFIG_USE_USER_AND_PASSWORD
    client.connect(clientID.c_str(), mqttUserName, mqttPassword); // first is the for user separation
#else                                                                // second two are for authentication
    client.connect(clientID.c_str());                             // no encryption on connection
#endif

    Serial.print("MQTT connecting");
    while (!client.connected()) {                         
      Serial.print(".");                                  
      delay(pause_between_checks);

      if (millis() - start_milis > WIFI_TIMEOUT_TIME) { // reset if we haven't gotten it yet
        Serial.println("\nResetting...");               // this will go back and start over
        ESP.restart();
      }
    }
    Serial.println("\nMQTT connected.");
  }
}
