// rtc_eeprom.h
#ifndef RTC_EEPROM_H
#define RTC_EEPROM_H

#include <Arduino.h>
#include <RTClib.h>

// objek RTC (definisi ada di rtc_eeprom.ino)
extern RTC_DS1307 rtc;

// fungsi untuk read/write json di EEPROM (implement di rtc_eeprom.ino)
extern String readJsonFromEEPROM();
extern bool writeJsonToEEPROM(const String &s);

// fungsi untuk inisialisasi RTC + EEPROM
extern void initRTCAndLoadAlarms();

// set RTC dari string "YYYY-MM-DD" & "HH:MM"
extern bool setRTCFromStrings(const char* dateStr, const char* timeStr);

#endif // RTC_EEPROM_H
