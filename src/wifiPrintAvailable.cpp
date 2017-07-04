#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "conf.h"

void wifiPrintAvailableNetworks() {
  int numNets = WiFi.scanNetworks(false, false); // bool async, bool show_hidden -> return int
  for (size_t i = 0; i < numNets; i++) {
    Serial.print(WiFi.SSID(i));
    // Serial.print(" | ");
    Serial.println(WiFi.RSSI(i));
  }
}
