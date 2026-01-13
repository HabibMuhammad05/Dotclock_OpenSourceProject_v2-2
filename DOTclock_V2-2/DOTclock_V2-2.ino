/*-----------ESP8266 max7219 Panel Clock With RTC &Alarm----------------*/
/*---------------Wemos D1 - 32x8Px MAX7219 LED Matrix-------------------*/
/*-----------------Source Code by -              -----------------------*/
/*-------------Modified & Adapted by DOTClock Team (PMW)----------------*/
/*------------------------------V2.2------------------------------------*/
/*----------------------------------------------------------------------*/

// =======================================================================
// Define DEBUG to enable debugging(Serial Monitor); comment it out to disable
// =======================================================================
#define DEBUG

#ifdef DEBUG
  #define DEBUG2_PRINT(...) Serial.printf(__VA_ARGS__)
  #define DEBUG2_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG2_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG2_PRINT(...)    // Do nothing
  #define DEBUG2_PRINTLN(...)  // Do nothing
  #define DEBUG2_BEGIN(baud)   // Do nothing
#endif

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "rtc_eeprom.h"   
#include "webpage.h"      
#include <EEPROM.h>      

#include <PrayerTimes.h>
double times[sizeof(TimeName)/sizeof(char*)];

#define DHTPIN        D4
#define ALARM_OUT_PIN D5

float g_lastTemp = NAN;
float g_lastHum = NAN;
unsigned long g_lastSensorMillis     = 0;
const unsigned long SENSOR_INTERVAL  = 5UL * 1000UL; 
unsigned long lastDisplay            = 0;
const unsigned long DISPLAY_INTERVAL = 2000; 


float latitudeData = -6.16;
float longitudeData = 106.61;
uint8_t ihti = 2; 

void startAP();
void initServer();
void handleClientLoop();
void readSensorOnce();
void checkAlarmsLoop();
void setupOTA();
void handleOTA();
void initRTCAndLoadAlarms();
void JadwalSholat();

void setup() {
  DEBUG2_BEGIN(115200);
  startDisplay();
  pinMode(ALARM_OUT_PIN, OUTPUT);
  digitalWrite(ALARM_OUT_PIN, LOW);

  startAP();
  readSensorOnce();
  initRTCAndLoadAlarms();
  setupOTA();
  initServer();
  loadTextFromEEPROM();

  DEBUG2_PRINTLN("Setup complete.");
}

void loop() {
  displayClock();
  
  unsigned long nowMs = millis();
  if (nowMs - g_lastSensorMillis >= SENSOR_INTERVAL) {
    g_lastSensorMillis = nowMs;
    readSensorOnce();
    getTempHum();
  }

  if (millis() - lastDisplay >= 2000) {
    lastDisplay = millis();
    printDateTime();
    JadwalSholat();
  }

  checkAlarmsLoop();
  handleClientLoop();
  handleOTA();

  delay(10);
}
