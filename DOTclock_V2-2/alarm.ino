// alarm.ino
#include <Arduino.h>
#include <ArduinoJson.h>
#include "rtc_eeprom.h"

// Forward declaration - printStringWithShift defined in max7219.h (included in clock.ino)
extern void printStringWithShift(const char* s, int shiftDelay);

// Variabel global dari file lain
extern float g_lastTemp;
extern float g_lastHum;

// Mencegah pemicu ulang di menit yang sama
static int lastTriggeredMinute = -1;

// -----------------------------------------------------------------------------
// checkAlarmsLoop()
// Dibuat untuk dipanggil di loop() — membaca daftar alarm dari EEPROM,
// memeriksa apakah ada alarm aktif yang waktunya sama dengan RTC sekarang,
// dan menyalakan output alarm selama 1 detik sambil tetap melayani client web.
// -----------------------------------------------------------------------------
void checkAlarmsLoop() {
  // Baca JSON alarm dari EEPROM
  String s = readJsonFromEEPROM();
  if (s.length() < 5) return;

  // Parse JSON
  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, s);
  if (err) {
    DEBUG2_PRINTLN("Alarm JSON parse error");
    return;
  }

  if (!doc.containsKey("alarms")) return;

  // Ambil waktu sekarang
  DateTime now = rtc.now();
  int hh = now.hour();
  int mm = now.minute();
  int currentMinuteKey = hh * 60 + mm;

  // Jika sudah dipicu di menit ini, lewati
  if (lastTriggeredMinute == currentMinuteKey) return;

  // Periksa setiap alarm
  const JsonArray alarms = doc["alarms"].as<JsonArray>();
  for (JsonObject alarm : alarms) {
    const char* timeStr = alarm["alarm_time"];     // "HH:MM"
    bool active = alarm["is_active"] | true;       // default true jika tidak ada
    if (!timeStr) continue;

    int ah = 0, am = 0;
    if (sscanf(timeStr, "%d:%d", &ah, &am) != 2) continue;
    if (!active) continue;

    if (ah == hh && am == mm) {
      DEBUG2_PRINT("Alarm triggered at %02d:%02d\n", hh, mm);
      digitalWrite(ALARM_OUT_PIN, HIGH);
      // Tampilkan teks peringatan pada LED matrix ketika buzzer menyala
      printStringWithShift("ALARM!!!", 40);

      // Tetap layani client web selama durasi alarm agar tidak freeze
      unsigned long endMs = millis() + 2000; // 1 detik
      while (millis() < endMs) {
        handleClientLoop();
        delay(1);
      }

      digitalWrite(ALARM_OUT_PIN, LOW);

      // Catat agar tidak terpicu ulang di menit yang sama
      lastTriggeredMinute = currentMinuteKey;
      break;
    }
  }
}
