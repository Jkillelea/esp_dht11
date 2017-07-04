#ifndef CONFIG_H_
#define CONFIG_H_

// fill these out
const char ssid[]                    = "XXXXX";
const char pass[]                    = "XXXXX";
const char clientID[]                = "XXXXX";
const char mqttUserName[]            = "XXXXX";
const char mqttPassword[]            = "XXXXX";
const int  mqttPortNum               = 13382;
const char mqttHostName[]            = "XXXXX.XXXXX.XXXXX";
const char PUBLISH_TOPIC[]           = "/data";
const unsigned int WIFI_TIMEOUT_TIME = 60*1000; // millis
const unsigned int STARTUP_DELAY     = 3*1000;  // miliseconds
const unsigned int LOOP_WAIT_TIME    = 3*1000; // miliseconds

#endif /* end of include guard: CONFIG_H_ */
