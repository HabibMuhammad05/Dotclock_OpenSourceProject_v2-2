// rtc_eeprom.ino
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "rtc_eeprom.h"


// Definisi nyata RTC
RTC_DS1307 rtc;

//int h, m, s, NTPday, NTPmonth, NTPyear, NTPdayOfWeek;

// Konstanta untuk EEPROM
const int EEPROM_SIZE   = 1024;
const int EEPROM_HEADER = 2;  // dua byte pertama menyimpan panjang data

// Array nama hari dalam bahasa Indonesia
const char* hari[7] = {
  "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"
};

// -----------------------------------------------------------------------------
// Inisialisasi RTC dan memuat data alarm dari EEPROM
// -----------------------------------------------------------------------------
void initRTCAndLoadAlarms() {
  Wire.begin();

  if (!rtc.begin()) {
    DEBUG2_PRINTLN("RTC not found!");
  } else {
    DEBUG2_PRINTLN("RTC OK.");

    if (!rtc.isrunning()) {
      DEBUG2_PRINTLN("RTC belum berjalan!");
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // opsional: set waktu compile
    }
  }

  // Inisialisasi EEPROM
  EEPROM.begin(EEPROM_SIZE);
  DEBUG2_PRINTLN("EEPROM initialized.");

  // Tampilkan data alarm dari EEPROM (opsional)
  String s = readJsonFromEEPROM();
  if (s.length() > 10) {
    DEBUG2_PRINT("Loaded alarms JSON: ");
    DEBUG2_PRINTLN(s);
  }
}

// -----------------------------------------------------------------------------
// Membaca data JSON dari EEPROM
// -----------------------------------------------------------------------------
String readJsonFromEEPROM() {
  uint16_t len = ((uint16_t)EEPROM.read(0) << 8) | EEPROM.read(1);
  if (len == 0xFFFF || len == 0 || len > EEPROM_SIZE - EEPROM_HEADER) return "";

  String s;
  s.reserve(len);

  for (int i = 0; i < len; i++) {
    char c = EEPROM.read(EEPROM_HEADER + i);
    s += c;
  }
  return s;
}

// -----------------------------------------------------------------------------
// Menulis data JSON ke EEPROM
// -----------------------------------------------------------------------------
bool writeJsonToEEPROM(const String& s) {
  int len = s.length();

  if (len > EEPROM_SIZE - EEPROM_HEADER) {
    DEBUG2_PRINTLN("EEPROM: payload too big");
    return false;
  }

  EEPROM.write(0, (len >> 8) & 0xFF);
  EEPROM.write(1, len & 0xFF);

  for (int i = 0; i < len; i++) {
    EEPROM.write(EEPROM_HEADER + i, s[i]);
  }

  EEPROM.commit();
  DEBUG2_PRINT("EEPROM: wrote %d bytes\n", len);
  return true;
}

// -----------------------------------------------------------------------------
// Mengatur RTC berdasarkan data string dari web (tanggal & waktu)
// -----------------------------------------------------------------------------
bool setRTCFromStrings(const char* dateStr, const char* timeStr) {
  int yyyy, mm, dd, hh, min;

  if (sscanf(dateStr, "%d-%d-%d", &yyyy, &mm, &dd) != 3) return false;
  if (sscanf(timeStr, "%d:%d", &hh, &min) != 2) return false;

  DateTime dt(yyyy, mm, dd, hh, min, 0);
  rtc.adjust(dt);

  // Ambil waktu setelah diset
  DateTime now = rtc.now();
  int hariIndex = now.dayOfTheWeek();

  DEBUG2_PRINTLN("====================================");
  DEBUG2_PRINTLN("RTC telah diset oleh web!");
  DEBUG2_PRINT("Tanggal: %02d/%02d/%04d\n", now.day(), now.month(), now.year());
  DEBUG2_PRINT("Jam:     %02d:%02d:%02d\n", now.hour(), now.minute(), now.second());
  DEBUG2_PRINT("Hari:    %s\n", hari[hariIndex]);
  DEBUG2_PRINTLN("====================================");

  return true;
}

// -----------------------------------------------------------------------------
// Menampilkan waktu & tanggal RTC di Serial Monitor
// -----------------------------------------------------------------------------
void printDateTime() {
  DateTime now = rtc.now();
  
  h = now.hour();
  m = now.minute();
  s = now.second();
  NTPday = now.day();
  NTPdayOfWeek = now.dayOfTheWeek();
  NTPmonth = now.month();
  NTPyear = now.year();
    
  DEBUG2_PRINT(
    "%s, %02d/%02d/%04d %02d:%02d:%02d\n",
    hari[now.dayOfTheWeek()],
    now.day(),
    now.month(),
    now.year(),
    now.hour(),
    now.minute(),
    now.second()
  );
  
}

void JadwalSholat() {

  set_calc_method(Karachi);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  get_prayer_times(NTPyear, NTPmonth, NTPday, latitudeData, longitudeData, 7, times);

}

  /*
    CALCULATION METHOD
    ------------------
    Jafari,   // Ithna Ashari
    Karachi,  // University of Islamic Sciences, Karachi
    ISNA,     // Islamic Society of North America (ISNA)
    MWL,      // Muslim World League (MWL)
    Makkah,   // Umm al-Qura, Makkah
    Egypt,    // Egyptian General Authority of Survey
    Custom,   // Custom Setting

    JURISTIC
    --------
    Shafii,    // Shafii (standard)
    Hanafi,    // Hanafi

    ADJUSTING METHOD
    ----------------
    None,        // No adjustment
    MidNight,   // middle of night
    OneSeventh, // 1/7th of night
    AngleBased, // angle/60th of night

    TIME IDS
    --------
    Fajr,
    Sunrise,
    Dhuhr,
    Asr,
    Sunset,
    Maghrib,
    Isha
  
  */
