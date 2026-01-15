
#ifndef RTC_EEPROM_H
#define RTC_EEPROM_H

#include <Arduino.h>
#include <RTClib.h>

extern RTC_DS1307 rtc;

extern String readJsonFromEEPROM();
extern bool writeJsonToEEPROM(const String &s);
extern void initRTCAndLoadAlarms();
extern bool setRTCFromStrings(const char* dateStr, const char* timeStr);

#endif 
