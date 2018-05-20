#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <DHT.h>

#include "conf.h"

WiFiClient net;
MQTTClient client;

#ifdef _BOARD_NODEMCU_
DHT dht(D7, DHT11);
#else // Huzzah board
DHT dht(13, DHT11);
#endif

// callback, but we do nothing
void messageReceived(String topic, String data, char* payloadBytes, unsigned int len) {}

void setup() {
  delay(STARTUP_DELAY);
  Serial.begin(115200);
  Serial.println("");
  wifiPrintAvailableNetworks();

  WiFi.mode(WIFI_STA);
  // WiFi.disconnect(false); // need to call before WiFi.begin()
  WiFi.begin(WIFI_SSID, WIFI_PW);
  connect();

  dht.begin();
}

void loop() {
  if ((WiFi.status() != WL_CONNECTED) || !client.connected()) // reconnect if lost
    connect();

  String id = clientID;

  client.publish(id + "/humidity",    (String) dht.readHumidity());
  client.publish(id + "/temperature", (String) dht.readTemperature());
  client.publish(id + "/ip",          (String) WiFi.localIP().toString());
  client.publish(id + "/rssi",        (String) WiFi.RSSI());

  delay(LOOP_WAIT_TIME);
}

// Tries to connect to WiFi and MQTT server
// if connecting to WiFi times out, the chip will reset in an attempt to
// connect to wifi again
void connect() {
  const int pause_between_checks = 500; // milliseconds
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

void wifiPrintAvailableNetworks() {
  int numNets = WiFi.scanNetworks(false, true); // bool async, bool show_hidden -> return int
  for (size_t i = 0; i < numNets; i++) {
    Serial.print(WiFi.SSID(i));
    Serial.print(" : ");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm");
  }
}
