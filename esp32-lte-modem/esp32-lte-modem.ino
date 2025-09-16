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

// ========== LIBRARIES =======================
#include "modem.h"
#include <ArduinoHttpClient.h>

// ========== global variables ================

// ========== objects =========================
HttpClient http(client, HTTP_SERVER, HTTP_PORT);


// ========== initial setup ===================
void setup(){
  Serial.begin(115200);
  delay(10);

  modemSetup();
}

// ========== MAIN CODE =======================
void loop(){

  modemConnect();
  
  Serial.print("HTTP GET request... ");
  uint16_t err = http.get(HTTP_RESOURCE);
  if (err != 0) {
    Serial.print("GET request failed: ");
    Serial.println(err);
    delay(10000);
    return;
  }

  uint16_t status = http.responseStatusCode();
  Serial.print("Status: ");
  Serial.println(status);

  String body = http.responseBody();
  Serial.println("Response:");
  Serial.println(body);

  http.stop();
  Serial.println("Connection closed!");

  modemDisconnect();
  
  while (true) {delay(1000);}
}

// ========== SOURCES =========================
/*
 * https://github.com/vshymanskyy/TinyGSM/blob/master/examples/HttpClient/HttpClient.ino
 * https://randomnerdtutorials.com/esp32-https-requests-sim-card-sim7000g/
*/
