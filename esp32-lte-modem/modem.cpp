
// ========== LIBRARIES =======================
#include "modem.h"

// ========== objects =========================
TinyGsm modem(SerialModem);
TinyGsmClient modemClient(modem);

// ========== objects =========================
#if MODEM_NETWORK_MODE == MODEM_CONNECTION_TINY_GSM
  TinyGsm modem(SerialModem);
  TinyGsmClient modemClient(modem);
#endif
#if MODEM_NETWORK_MODE == MODEM_CONNECTION_PPP
  NetworkClient modemClient;
#endif


// ========== FUNC. IMPLEMENTATION ============
void modemConnect(){
  Serial.print("Connecting modem...");
  modem.gprsConnect(APN_NAME, APN_USER, APN_PASSWD);

  Serial.print("Waiting for network...");
  while (!modem.waitForNetwork()) {
    Serial.println(" failed");
    delay(1000);
  }
  Serial.println(" ok");
  modemNetworkStatus();
}

void modemDisconnect(){
  modem.gprsDisconnect();
  SerialModem.println("Modem disconnected");
}

void modemGpioInit(){
  pinMode(MODEM_PIN_PWR, OUTPUT);
  digitalWrite(MODEM_PIN_PWR, LOW);
  pinMode(MODEM_PIN_SLEEP, OUTPUT);
  digitalWrite(MODEM_PIN_PWR, LOW);
}

void modemInfo(){
  Serial.print("Modem Info: ");
  Serial.println(modem.getModemInfo());
}

void modemPowerOn() {
  Serial.println("Modem power on");
  digitalWrite(MODEM_PIN_PWR, LOW);
  delay(MODEM_GPIO_PWR_TON_MS);
  digitalWrite(MODEM_PIN_PWR, HIGH);
}

void modemPowerOff(){
  Serial.println("Modem power off");
  digitalWrite(MODEM_PIN_PWR, LOW);
  delay(MODEM_GPIO_PWR_TOFF_MS);
  digitalWrite(MODEM_PIN_PWR, HIGH);
}

void modemForceRestart(){                                       // soft restart avaible in modem.restart() from TinyGSM
  Serial.println("Modem force restart");
  digitalWrite(MODEM_PIN_PWR, LOW);
  delay(MODEM_GPIO_RESET_TOFF_MS);
  digitalWrite(MODEM_PIN_PWR, HIGH);
}

void modemNetworkStatus(){
  Serial.println("Modem Status: ");
  Serial.print("\tIP: ");
  Serial.println(modem.localIP());

  Serial.print("\tIMSA: ");
  Serial.println(modem.getIMSI());

  Serial.print("\tCCID: ");
  Serial.println(modem.getSimCCID());

  Serial.print("\tISP Operator:");
  Serial.println(modem.getOperator());

  Serial.print("\tCSQ: ");
  Serial.println(modem.getSignalQuality());
}

void modemSleepOn(){
  Serial.println("Modem sleep mode requested");
  modem.sendAT("+CSCLK=1");
  digitalWrite(MODEM_PIN_SLEEP, HIGH);
}

void modemSleepOff(){
  Serial.println("Modem sleep mode exit requested");
  digitalWrite(MODEM_PIN_SLEEP, LOW); 
}

void modemSetup(){
  modemGpioInit();
  modemPowerOn();

  SerialModem.begin(MODEM_UART_BAUD, SERIAL_8N1, MODEM_PIN_RX, MODEM_PIN_TX);
  delay(MODEM_UART_READY_TIME_MS);

  Serial.println("Initializing modem...");
  modem.restart();
  // modem.init();

  modemInfo();

  if (SIM_PIN && modem.getSimStatus() != 3) {
    modem.simUnlock(SIM_PIN);
  }
}
