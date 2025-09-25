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
#ifndef MODEM_PIN_PWR
  #define MODEM_PIN_PWR         26
#endif
#ifndef MODEM_PIN_SLEEP                         // UART DTR SIMCom's pin
  #define MODEM_PIN_SLEEP       25
#endif
#ifndef SerialModem
  #define SerialModem           Serial1
#endif

// ========== MODEM SETUP =====================
#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_RX_BUFFER        1024
#define MODEM_UART_READY_TIME_MS  12
#define MODEM_GPIO_PWR_TON_MS     100
#define MODEM_GPIO_PWR_TOFF_MS    2600                  // 2500ms from manual + 100ms for safety       
#define MODEM_GPIO_RESET_TOFF_MS  10    

#define ISP_CLARO                 0x01
#define ISP_OI                    0x02
#define ISP_VIVO                  0x03
#define ISP_OI                    0x04

#define APN_ISP                   ISP_VIVO

#if APN_ISP == ISP_CLARO
  #define APN_NAME                "claro.com.br"
  #define APN_USER                "claro"
  #define APN_PASSWD              "claro"
#elif APN_ISP == ISP_OI
  #define APN_NAME                "gprs.oi.com.br"
  #define APN_USER                "oi"
  #define APN_PASSWD              "oi"
#elif APN_ISP == ISP_TIM
  #define APN_NAME                "timbrasil.br"
  #define APN_USER                "tim"
  #define APN_PASSWD              "tim"
#elif APN_ISP == ISP_VIVO
  #define APN_NAME                "zap.vivo.com.br"
  #define APN_USER                "vivo"
  #define APN_PASSWD              "vivo"
#else
  #error "APN settings missing!"
#endif

#define SIM_PIN                 0000                // 0000 - NO PIN REQUIRED

// ========== LIBRARIES =======================
#include <Arduino.h>
#include <TinyGsmClient.h>

// ========== objects =========================
extern TinyGsm modem;
extern TinyGsmClient gsmClient;

// ========== function prototypes =============
void modemConnect();
void modemDisconnect();
void modemPowerOn();
void modemPowerOff();
void modemForceRestart();
void modemSleepOn();
void modemSleepOff();
void modemSetup();


#endif // MODEM_H
