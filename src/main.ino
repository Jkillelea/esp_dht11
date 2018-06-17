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
void messageReceived(String topic, String data, char* payloadBytes, uint32_t len) {}

void setup() {
  D delay(STARTUP_DELAY);
  D Serial.begin(115200);
  D Serial.println("");
  D wifiPrintAvailableNetworks();

  // WiFi.mode(WIFI_STA);
  // WiFi.persistent(true);
  D Serial.println("dht.begin");
  dht.begin();

  D Serial.println("WiFi.begin");
  WiFi.begin(WIFI_SSID, WIFI_PW);

  D Serial.println("connect");
  connect();

  D Serial.println("loop");
  loop();

  D Serial.println("client.disconnect");
  client.disconnect();

  D Serial.println("ESP.deepSleep");
  ESP.deepSleep(SLEEP_TIME);
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
  const uint32_t pause_between_checks = 500; // milliseconds
  uint64_t start_milis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    D Serial.println("\nWiFi not connected");
    D Serial.print("connecting to " + String(WIFI_SSID));
  
    // wifi connection
    while (WiFi.status() != WL_CONNECTED) {
      D Serial.print(".");
      delay(pause_between_checks);

      if (millis() - start_milis > WIFI_TIMEOUT_TIME) { // reset if we haven't gotten it yet
        D Serial.println("\nResetting...");               // this will go back and start over
        ESP.restart();
      }
    }
    D Serial.print("\nWiFi connected. IP: ");
    D Serial.print(WiFi.localIP().toString());
    D Serial.print(" RSSI: ");
    D Serial.println(WiFi.RSSI());
  }

  // mqtt server connection
  client.begin(mqttHostName, mqttPortNum, net);
  if (!client.connected()) {
    D Serial.println("\nMQTT not connected");

#if CONFIG_USE_USER_AND_PASSWORD
    client.connect(clientID.c_str(), mqttUserName, mqttPassword); // first is the for user separation
#else                                                                // second two are for authentication
    client.connect(clientID.c_str());                             // no encryption on connection
#endif

    D Serial.print("MQTT connecting");
    while (!client.connected()) {                         
      D Serial.print(".");                                  
      delay(pause_between_checks);

      if (millis() - start_milis > WIFI_TIMEOUT_TIME) { // reset if we haven't gotten it yet
        D Serial.println("\nResetting...");               // this will go back and start over
        ESP.restart();
      }
    }
    D Serial.println("\nMQTT connected.");
  }
}

void wifiPrintAvailableNetworks() {
  uint32_t numNets = WiFi.scanNetworks(false, true); // bool async, bool show_hidden -> return int
  Serial.println("== SSID : RSSI ==");
  for (uint32_t i = 0; i < numNets; i++) {
    String ssid = WiFi.SSID(i);
    Serial.print(ssid != "" ? ssid : "[Hidden]");
    Serial.print(" : ");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm");
  }
  Serial.println("======");
}
