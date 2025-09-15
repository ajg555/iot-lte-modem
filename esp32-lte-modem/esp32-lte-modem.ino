/*
  Code for ESP32 uC family
  Modem: SIMCom A7608SA-H
  HTTP (only) test application 
  HTTPS not support on TinyGsm yet for this module (09/2025) - https://github.com/vshymanskyy/TinyGSM/tree/master  
  WARNING: DO NOT USE THE MODULE WITHOUT AN ANTENNA!!!
*/

// ========== BOARD SETUP =====================
#define UART_BAUD               115200
#define PIN_TX                  17
#define PIN_RX                  16
#define PWR_PIN                 26

// ========== MACROS AND CONSTANTS ============
#define HTTP_SERVER             "example.com"
#define HTTP_PORT               80
#define HTTP_RESOURCE           "/"

// ========== MODEM SETUP =====================
#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_RX_BUFFER      1024
#define ISP_CLARO               0x01
#define ISP_OI                  0x02
#define ISP_VIVO                0x03
#define ISP_OI                  0x04
#define APN_ISP                 ISP_VIVO
#if APN_ISP == ISP_CLARO
  #define APN_NAME              "claro.com.br"
  #define APN_USER              "claro"
  #define APN_PASSWD            "claro"
#elif APN_ISP == ISP_OI
  #define APN_NAME              "gprs.oi.com.br"
  #define APN_USER              "oi"
  #define APN_PASSWD            "oi"
#elif APN_ISP == ISP_TIM
  #define APN_NAME              "timbrasil.br"
  #define APN_USER              "tim"
  #define APN_PASSWD            "tim"
#elif APN_ISP == ISP_VIVO
  #define APN_NAME              "zap.vivo.com.br"
  #define APN_USER              "vivo"
  #define APN_PASSWD            "vivo"
#else
  #error "APN settings missing!"
#endif

#define SIM_PIN                 "0000"
#define SerialModem             Serial1

// ========== LIBRARIES =======================
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

// ========== function prototypes =============
void modemPowerOn();
void modemPowerOff();
void modemRestart();

// ========== global variables ================

// ========== objects =========================
TinyGsm modem(SerialModem);
TinyGsmClient client(modem);
HttpClient http(client, HTTP_SERVER, HTTP_PORT);


// ========== initial setup ===================
void setup(){
  Serial.begin(115200);
  delay(10);

  modemPowerOn();

  SerialModem.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(6000);

  Serial.println("Initializing modem...");
  modem.restart();
  modemRestart();
  // modem.init();

  delay(5000);

  Serial.print("Modem Info: ");
  Serial.println(modem.getModemInfo());

}

// ========== MAIN CODE =======================
void loop(){

  modem.gprsConnect(APN_NAME, APN_USER, APN_PASSWD);

  Serial.print("Waiting for network...");
  while (!modem.waitForNetwork()) {
    Serial.println(" failed");
    delay(1000);
  }
  Serial.println(" ok");
  
  Serial.print("HTTP GET request... ");
  int err = http.get(HTTP_RESOURCE);
  if (err != 0) {
    Serial.print("GET request failed: ");
    Serial.println(err);
    delay(10000);
    return;
  }

  int status = http.responseStatusCode();
  Serial.print("Status: ");
  Serial.println(status);

  String body = http.responseBody();
  Serial.println("Response:");
  Serial.println(body);

  http.stop();
  Serial.println("Connection closed!");
  
  while (true) {delay(1000);}
}

// ========== FUNC. IMPLEMENTATION ============
void modemPowerOn() {
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);
  digitalWrite(PWR_PIN, HIGH);
}

void modemPowerOff(){
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);
  digitalWrite(PWR_PIN, HIGH);
}

void modemRestart(){
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}

// ========== SOURCES =========================
/*
 * https://github.com/vshymanskyy/TinyGSM/blob/master/examples/HttpClient/HttpClient.ino
 * https://randomnerdtutorials.com/esp32-https-requests-sim-card-sim7000g/
*/
