#ifndef MODEM_H
#define MODEM_H


// ========== MODEM GPIO ======================
#ifndef MODEM_UART_BAUD
  #define MODEM_UART_BAUD       115200
#endif
#ifndef MODEM_PIN_TX
  #define MODEM_PIN_TX          17
#endif
#ifndef MODEM_PIN_RX
  #define MODEM_PIN_RX          16
#endif
#ifndef MODEM_PWR_PIN
  #define MODEM_PWR_PIN         26
#endif
#ifndef SerialModem
  #define SerialModem           Serial1
#endif

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

#define SIM_PIN                 0000                // 0000 - NO PIN REQUIRED

// ========== LIBRARIES =======================
#include <Arduino.h>
#include <TinyGsmClient.h>

// ========== objects =========================
extern TinyGsm modem;
extern TinyGsmClient client;


// ========== function prototypes =============
void modemConnect();
void modemDisconnect();
void modemPowerOn();
void modemPowerOff();
void modemRestart();
void modemSetup();





















#endif // MODEM_H
