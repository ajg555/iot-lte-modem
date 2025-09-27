

#define PPP_MODEM_APN "zap.vivo.com.br"
#define PPP_MODEM_PIN "0000"  // or NULL

// ESP32 BOARD
#define ESP32_DEV_KIT_1

#ifdef ESP32_DEV_KIT_1
#define MODEM_SERIAL    2
#define MODEM_BAUDRATE  115200
#define MODEM_TX        17
#define MODEM_RX        16
#define MODEM_SLEEP     25
#else
#error "Invalid BOARD GPIO setup!"
#endif

// ========== MACROS AND CONSTANTS ============
#define HTTP_SERVER             "example.com"
#define HTTP_PORT               80
#define HTTP_RESOURCE           "/"
#define LOOP_DELAY              15000

// SIMCom A7608SA-H basic module with just TX,RX and RST
#define PPP_MODEM_TX      MODEM_TX
#define PPP_MODEM_RX      MODEM_RX
#define PPP_MODEM_RTS     -1
#define PPP_MODEM_CTS     -1
#define PPP_MODEM_FC      ESP_MODEM_FLOW_CONTROL_NONE
#define PPP_MODEM_MODEL   PPP_MODEM_GENERIC

// ========== LIBRARIES =======================
#include "modem.h"
#include <PPP.h>
#include <ArduinoHttpClient.h>

// ========== objects =========================
NetworkClient modemClient;
HttpClient http(modemClient, HTTP_SERVER, HTTP_PORT);

void onEvent(arduino_event_id_t event, arduino_event_info_t info) {
  switch (event) {
    case ARDUINO_EVENT_PPP_START:        Serial.println("PPP Started"); break;
    case ARDUINO_EVENT_PPP_CONNECTED:    Serial.println("PPP Connected"); break;
    case ARDUINO_EVENT_PPP_GOT_IP:       Serial.println("PPP Got IP"); break;
    case ARDUINO_EVENT_PPP_LOST_IP:      Serial.println("PPP Lost IP"); break;
    case ARDUINO_EVENT_PPP_DISCONNECTED: Serial.println("PPP Disconnected"); break;
    case ARDUINO_EVENT_PPP_STOP:         Serial.println("PPP Stopped"); break;
    default:                             break;
  }
}

void setup() {
  Serial.begin(115200);

  // Listen for modem events
  Network.onEvent(onEvent);

  // Configure the modem
  PPP.setApn(PPP_MODEM_APN);
  //PPP.setPin(PPP_MODEM_PIN);
  //PPP.setResetPin(PPP_MODEM_RST, PPP_MODEM_RST_LOW, PPP_MODEM_RST_DELAY);
  PPP.setPins(PPP_MODEM_TX, PPP_MODEM_RX, PPP_MODEM_RTS, PPP_MODEM_CTS, PPP_MODEM_FC);

  Serial.println("Starting the modem. It might take a while!");
  PPP.begin(PPP_MODEM_MODEL);

  Serial.print("Manufacturer: ");
  Serial.println(PPP.cmd("AT+CGMI", 10000));
  Serial.print("Model: ");
  Serial.println(PPP.moduleName());
  Serial.print("IMEI: ");
  Serial.println(PPP.IMEI());

  bool attached = PPP.attached();
  if (!attached) {
    int i = 0;
    unsigned int s = millis();
    Serial.print("Waiting to connect to network");
    while (!attached && ((++i) < 600)) {
      Serial.print(".");
      delay(100);
      attached = PPP.attached();
    }
    Serial.print((millis() - s) / 1000.0, 1);
    Serial.println("s");
    attached = PPP.attached();
  }

  Serial.print("Attached: ");
  Serial.println(attached);
  Serial.print("State: ");
  Serial.println(PPP.radioState());
  if (attached) {
    Serial.print("Operator: ");
    Serial.println(PPP.operatorName());
    Serial.print("IMSI: ");
    Serial.println(PPP.IMSI());
    Serial.print("RSSI: ");
    Serial.println(PPP.RSSI());
    int ber = PPP.BER();
    if (ber > 0) {
      Serial.print("BER: ");
      Serial.println(ber);
      Serial.print("NetMode: ");
      Serial.println(PPP.networkMode());
    }

    Serial.println("Switching to data mode...");
    PPP.mode(ESP_MODEM_MODE_CMUX);  // Data and Command mixed mode
    if (!PPP.waitStatusBits(ESP_NETIF_CONNECTED_BIT, 1000)) {
      Serial.println("Failed to connect to internet!");
    } else {
      Serial.println("Connected to internet!");
    }
  } else {
    Serial.println("Failed to connect to network!");
  }
}

void loop() {
  if (PPP.connected()) {
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
  }
  delay(20000);
}
