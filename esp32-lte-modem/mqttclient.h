
#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H


// ========== MQTT SETUP ======================
#ifndef MQTT_PORT
  #define MQTT_PORT                     1883
#endif
#ifndef MQTT_SERVER
  #define MQTT_SERVER                   "test.mosquitto.org"
#endif
#define MQTT_WITH_AUTH                  0x01
#define MQTT_WITHOUT_AUTH               0x02
#define MQTT_AUTH_MODE                  MQTT_WITHOUT_AUTH

#if MQTT_AUTH_MODE == MQTT_WITH_AUTH
  #if !defined(MQTT_USER) || !defined(MQTT_PASSWD)
    #error "MQTT with authentication enabled: insert the credentials into your secret files using MQTT_USER and MQTT_PASSWD macros!"
  #endif
#endif

#ifndef MQTT_MBUFFER
  #define MQTT_MBUFFER                  256
#endif
#ifndef MQTT_CLIENT
  #define MQTT_CLIENT                   "ESP32_MODEM_SIMCom"      // host MQTT client - must be unique
#endif

#define MQTT_KEEPALIVE                  60
#define MQTT_TIMEOUT                    60                       
#define MQTT_CONNECT_RETRY              3                              
#define MQTT_CONNECT_RETRY_MAX          100                            
#define MQTT_RECONNECT_TIME_INTERVAL    5000                           
#define MQTT_MESSAGE_RETAIN_TRUE        1                              
#define MQTT_MESSAGE_RETAIN_FALSE       0
#define MQTT_JSON_KEY_MAX_SIZE          15

#define MQTT_TOPIC_EXAMPLE              "esp32/a76"
#define MQTT_TOPIC_EXAMPLE_CALLBACK     "esp32/a76/cb"

#define NETWORK_RESET_ENABLED           1
#define NETWORK_RESET_DISABLED          0

#define JSON_DOCUMENT_SIZE              256

// ========== LIBRARIES =======================
//#include "env.h"
#include "modem.h"
#include<ArduinoJson.h>
#include<PubSubClient.h>
#include<string.h>

// ========== objects =========================

// ========== function prototypes =============
void callback(char* topic, byte* message, unsigned int length);       
void mqttSetup();                                                     
void mqttSubscribe(char *topic);
void mqttConnect();                                                   
void mqttPublish(char *topic, char *payload, uint8_t retain);


#endif                                                                // MQTTCLIENT_H
