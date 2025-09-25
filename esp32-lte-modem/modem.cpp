
// ========== LIBRARIES =======================
#include "modem.h"

// ========== objects =========================
TinyGsm modem(SerialModem);
TinyGsmClient gsmClient(modem);


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

  Serial.print("Modem Info: ");
  Serial.println(modem.getModemInfo());

  if (SIM_PIN && modem.getSimStatus() != 3) {
    modem.simUnlock(SIM_PIN);
  }
}
