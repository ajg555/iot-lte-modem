
// ========== LIBRARIES =======================
#include "modem.h"

// ========== objects =========================
TinyGsm modem(SerialModem);
TinyGsmClient client(modem);


// ========== FUNC. IMPLEMENTATION ============
void modemConnect(){
  modem.gprsConnect(APN_NAME, APN_USER, APN_PASSWD);

  Serial.print("Waiting for network...");
  while (!modem.waitForNetwork()) {
    Serial.println(" failed");
    delay(1000);
  }
  Serial.println(" ok");
}

void modemDisconnect(){
  modem.gprsDisconnect();
  SerialModem.println("Modem disconnected");
}

void modemPowerOn() {
  pinMode(MODEM_PWR_PIN, OUTPUT);
  digitalWrite(MODEM_PWR_PIN, LOW);
  delay(1000);
  digitalWrite(MODEM_PWR_PIN, HIGH);
}

void modemPowerOff(){
  pinMode(MODEM_PWR_PIN, OUTPUT);
  digitalWrite(MODEM_PWR_PIN, LOW);
  delay(1500);
  digitalWrite(MODEM_PWR_PIN, HIGH);
}

void modemRestart(){
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}

void modemSetup(){
  modemPowerOn();

  SerialModem.begin(MODEM_UART_BAUD, SERIAL_8N1, MODEM_PIN_RX, MODEM_PIN_TX);
  delay(6000);

  Serial.println("Initializing modem...");
  modem.restart();
  modemRestart();
  // modem.init();

  delay(5000);

  Serial.print("Modem Info: ");
  Serial.println(modem.getModemInfo());

  if (SIM_PIN && modem.getSimStatus() != 3) {
    modem.simUnlock(SIM_PIN);
  }
}
