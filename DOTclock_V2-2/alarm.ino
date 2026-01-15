// alarm.ino
#include <Arduino.h>
#include <ArduinoJson.h>
#include "rtc_eeprom.h"

extern void printStringWithShift(const char* s, int shiftDelay);
extern float g_lastTemp;
extern float g_lastHum;
static int lastTriggeredMinute = -1;

void checkAlarmsLoop() {
  String s = readJsonFromEEPROM();
  if (s.length() < 5) return;
  
  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, s);
  if (err) {
    DEBUG2_PRINTLN("Alarm JSON parse error");
    return;
  }

  if (!doc.containsKey("alarms")) return;

  DateTime now = rtc.now();
  int hh = now.hour();
  int mm = now.minute();
  int currentMinuteKey = hh * 60 + mm;

  if (lastTriggeredMinute == currentMinuteKey) return;

  const JsonArray alarms = doc["alarms"].as<JsonArray>();
  for (JsonObject alarm : alarms) {
    const char* timeStr = alarm["alarm_time"];     
    bool active = alarm["is_active"] | true;       
    if (!timeStr) continue;

    int ah = 0, am = 0;
    if (sscanf(timeStr, "%d:%d", &ah, &am) != 2) continue;
    if (!active) continue;

    if (ah == hh && am == mm) {
      DEBUG2_PRINT("Alarm triggered at %02d:%02d\n", hh, mm);
      digitalWrite(ALARM_OUT_PIN, HIGH);
      printStringWithShift("ALARM!!!", 40);

      unsigned long endMs = millis() + 2000;
      while (millis() < endMs) {
        handleClientLoop();
        delay(1);
      }

      digitalWrite(ALARM_OUT_PIN, LOW);
      lastTriggeredMinute = currentMinuteKey;
      break;
    }
  }
}
