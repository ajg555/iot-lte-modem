/*
  Code for ESP32 uC family
  Arduino IDE 1.8.19
  Modem: SIMCom A7608SA-H
  HTTP (only) test application 
  HTTPS not support on TinyGsm yet for this module (09/2025) - https://github.com/vshymanskyy/TinyGSM/tree/master  
  WARNING: DO NOT USE THE MODULE WITHOUT AN ANTENNA!!!
*/

// ========== MACROS AND CONSTANTS ============
#define HTTP_SERVER             "example.com"
#define HTTP_PORT               80
#define HTTP_RESOURCE           "/"
#define LOOP_DELAY              15000

// ========== LIBRARIES =======================
#include "modem.h"
#include "mqttclient.h"
#include <ArduinoHttpClient.h>

// ========== GLOBAL VARIABLES ================
uint32_t runtime = 0;

// ========== OBJECTS =========================
HttpClient http(modemClient, HTTP_SERVER, HTTP_PORT);


// ========== INITIAL SETUP ===================
void setup(){
  Serial.begin(115200);
  delay(10);

  modemSetup();
  mqttSetup();
  modemConnect();
}

// ========== MAIN CODE =======================
void loop(){

  mqttConnect();
  
  if(millis() - runtime > LOOP_DELAY){    

    // ========================================
    //          HTTP request example
    // ========================================
    //  Serial.print("HTTP GET request... ");
    //  uint16_t err = http.get(HTTP_RESOURCE);
    //  if (err != 0) {
    //    Serial.print("GET request failed: ");
    //    Serial.println(err);
    //    delay(10000);
    //    return;
    //  }
    //
    //  uint16_t status = http.responseStatusCode();
    //  Serial.print("Status: ");
    //  Serial.println(status);
    //
    //  String body = http.responseBody();
    //  Serial.println("Response:");
    //  Serial.println(body);
    //
    //  http.stop();
    //  Serial.println("Connection closed!");
  
    mqttPublish(MQTT_TOPIC_EXAMPLE, "555", MQTT_MESSAGE_RETAIN_FALSE);
    
    // ========================================
    //          Modem reset test
    // ========================================
     modemPowerOff();
     delay(5000);
     modemPowerOn();
     modemConnect();
    
    mqttPublish(MQTT_TOPIC_EXAMPLE, "741", MQTT_MESSAGE_RETAIN_FALSE);

    // ========================================
    //          Modem disconnect
    // ========================================
    // modemDisconnect();
    
    runtime = millis();
  }
}

// ========== SOURCES =========================
/*
 * https://github.com/vshymanskyy/TinyGSM/blob/master/examples/HttpClient/HttpClient.ino
 * https://randomnerdtutorials.com/esp32-https-requests-sim-card-sim7000g/
*/
