float utcOffset = 7;                     // Time Zone setting

#define DIN_PIN 15                       // Data Pin  - NodeMCU D8
#define CS_PIN  13                       // Chip Sel  - NodeMCU D7
#define CLK_PIN 12                       // Clock In  - NodeMCU D6

bool enableTeks = true;
bool enableDHT  = false;
char teksBuf[80] = "DOTclock ";

int h, m, s, NTPday, NTPmonth, NTPyear, NTPdayOfWeek;
float temp;
float hum;
char txtDHT[30];
char monthNames[][5]  = {"JAN","FEB","MAR","APR","MEI","JUN","JUL","AGU","SEP","OKT","NOV","DES"};
char namaHari[][7]    = {"MINGGU", "SENIN","SELASA","RABU","KAMIS","JUMAT","SABTU"};
char txtScroll[80];
char txtAlarm[20];
static bool alternate = true;

//unsigned long clkTime = 0;
//unsigned long dotTime = 0;
//bool dots = false;
//bool del = false;

// From alarm.ino
bool alarmStat;

#include "fonts.h"
#include "max7219.h"

void startDisplay() {
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0);
}

void displayClock() {

  // ============================================================
  //   ALARM MODE — block everything except the alarm message
  // ============================================================
  if (alarmStat) {
    sprintf(txtAlarm, "ALARM HIDUP");
    printString(txtAlarm, fontBesar);   // IMPORTANT: big font
    return;                             // <- prevents overwriting alarm message
  }

  // ============================================================
  //   NORMAL CLOCK MODE
  // ============================================================
  if (millis() - clkTime > 30000 && !del && dots) {
    printDateTime();
    printStringWithShift("  ", 40); delay(5);

    if (enableTeks) {
      printStringWithShift(teksBuf, 40); 
      delay(5);
    }

    sprintf(txtScroll, " -- %s, %d %s %d -- ", 
          namaHari[NTPdayOfWeek], 
          NTPday, 
          monthNames[NTPmonth - 1], 
          NTPyear);
          
    printStringWithShift(txtScroll, 40); delay(10);
    
    sprintf(txtDHT, "T:%dC - H:%d%", (int)temp, (int)hum);
    if (enableDHT || alternate) {
      printStringWithShift(txtDHT, 40);
      printStringWithShift("               ", 40); delay(10);
      delay(5);
      alternate = false;
    }else{ 
      ScrollJadwalSholat(); 
      alternate = true;
    }



    clkTime = millis();
  }

  // blinking colon
  if (millis() - dotTime > 500) {
    dots = !dots;
    dotTime = millis();
  }

  showAnimClock();

  // ============================================================
  //   LED BRIGHTNESS CONTROL
  // ============================================================
  if ( (h == 0) || (h >= 1 && h <= 6) ) {
    sendCmdAll(CMD_INTENSITY, 0);
  }
  else if (h >= 18 && h <= 23) {
    sendCmdAll(CMD_INTENSITY, 1);
  }
  else {
    sendCmdAll(CMD_INTENSITY, 3);
  }
}

void loadTextFromEEPROM() {
  String json = readJsonFromEEPROM();
  if (json.length() == 0) return;

  DynamicJsonDocument doc(512);
  if (deserializeJson(doc, json) != DeserializationError::Ok) return;

  const char* newText = doc["custom_text"];
  if (newText) {
    strncpy(teksBuf, newText, sizeof(teksBuf) - 1);
    teksBuf[sizeof(teksBuf) - 1] = '\0';
  }
}
