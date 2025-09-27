
// ========== LIBRARIES =======================
#include "mqttclient.h"

// ========== objects =========================
PubSubClient mqttClient(MQTT_SERVER, MQTT_PORT, callback, modemClient);               // callback: mqtt topic's receive messages

// ========== global variables ================

// ========== FUNC. IMPLEMENTATION ============
void mqttSetup() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);                                       
  mqttClient.setCallback(callback);                                                   
  mqttClient.setKeepAlive(MQTT_KEEPALIVE);                                            
  mqttClient.setSocketTimeout(MQTT_TIMEOUT);                                        
}

void mqttConnect() {
  static uint8_t maxRetry = 0;
  uint8_t   retry = MQTT_CONNECT_RETRY;
  uint32_t  runtime = 0;
  
  if (!mqttClient.connected()) {                                                        
    do {
      Serial.printf("Connecting to MQTT Server (Broker: %s, Port: %d, rc = %d)\n", MQTT_SERVER, MQTT_PORT, mqttClient.state());

      #if MQTT_AUTH_MODE == MQTT_WITH_AUTH
        if (mqttClient.connect(MQTT_mqttClient, MQTT_USER, MQTT_PASSWD)) {
      #else
        if (mqttClient.connect(MQTT_CLIENT)) {                                            
      #endif
        Serial.println("MQTT connected!");
                                                                                    // topics to subscribe
        mqttSubscribe(MQTT_TOPIC_EXAMPLE_CALLBACK);
      } else {                                                                      
        Serial.printf("MQTT connection attempt failed (rc= %d), reconnect in %d seconds\n", mqttClient.state(), MQTT_RECONNECT_TIME_INTERVAL / 1000);
        runtime = millis();
        while (millis() - runtime < MQTT_RECONNECT_TIME_INTERVAL) {};               
      }
      retry--;
      maxRetry++;
      #if(NETWORK_RESET)
        if(maxRetry > MQTT_CONNECT_RETRY_MAX){
          Serial.println("MQTT reconnection limit reached. Restarting system!");
          ESP.restart();
        }
      #endif
    } while (!mqttClient.connected() && retry);
  }
  else {
    mqttClient.loop();                                                                  
  }
}

void mqttSubscribe(char *topic) {
  Serial.printf("MQTT: subscribing to topic – '%s'\n", topic);
  mqttClient.subscribe(topic);
}

void mqttPublish(char *topic, char *payload, uint8_t retain) {

  uint8_t messageRetain = MQTT_MESSAGE_RETAIN_FALSE;

  if (retain == MQTT_MESSAGE_RETAIN_TRUE)
    messageRetain = MQTT_MESSAGE_RETAIN_TRUE;

  mqttClient.publish(topic, payload, messageRetain);
  Serial.printf("MQTT sending message (RETAIN FLAG: %d):\n\tTopic: %s\n\tPayload: %s\n", messageRetain, topic, payload);
}

void callback(char* topic, byte* message, unsigned int length) {

  StaticJsonDocument<JSON_DOCUMENT_SIZE> doc;                                       // JSON doc
  DeserializationError error;                                                       // object for convert payload MQTT to JSON
  char payload[MQTT_MBUFFER] = "";

  if (length <= MQTT_MBUFFER) {                                                     // check max. buffer overflow
                                                                                    // convertendo message received for bytes to strings
    for (int i = 0; i < length; i++) {
      payload[i] = (char)message[i];
    }
    payload[length] = '\0';
    Serial.printf("MQTT message received: \n\tTopic: %s \n\tPayload: %s\n", topic, payload);

    error = deserializeJson(doc, payload);                                          // payload in JSON to ArduinoJson object

    if (error) {
      Serial.print("MQTT callback error: deserialize JSON error: ");
      Serial.println(error.f_str());
      return;
    }

                                                                                    // topic callback handler
    if(!strcmp(topic, MQTT_TOPIC_EXAMPLE_CALLBACK)){                                      
      mqttPublish(MQTT_TOPIC_EXAMPLE, "41", MQTT_MESSAGE_RETAIN_FALSE); 
    }
  }
  else {
    Serial.printf("Message on topic '%s' discarded: buffer overflow!\n", topic);
  }
}
