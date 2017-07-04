#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#include "conf.h"
#include "funcs.h"

WiFiClient            net;
MQTTClient            client;
DHT                   dht(D4, DHT11);
StaticJsonBuffer<200> jsonBuffer; // 200 bytes
JsonObject &jsonRoot = jsonBuffer.createObject();

// callback, but we do nothing
void messageReceived(String topic, String data, char* payloadBytes, unsigned int len) {}

void setup() {
    delay(STARTUP_DELAY);
    Serial.begin(115200);
    Serial.println("");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(false);
    WiFi.begin(ssid, pass);

    connect();
    dht.begin();
}

void loop() {
    String jsonString;

    if ((WiFi.status() != WL_CONNECTED) || !client.connected()) // reconnect if lost
        connect();

    jsonRoot["humidity"]     = dht.readHumidity();
    jsonRoot["temperature"]  = dht.readTemperature();
    jsonRoot["ip_addr"]      = WiFi.localIP().toString();
    jsonRoot["rssi"]         = WiFi.RSSI();
    jsonRoot["secs_elapsed"] = millis()/1000;

    jsonRoot.printTo(jsonString);

    jsonRoot.printTo(Serial);
    Serial.println("");

    client.publish(PUBLISH_TOPIC, jsonString);

    // take a nap
    // WiFi.forceSleepBegin();
    delay(LOOP_WAIT_TIME);
    // WiFi.forceSleepWake();
}
