// wifi_ap.ino
#include <ESP8266WiFi.h>

// SSID & password sesuai permintaan
const char* ap_ssid = "DOTclock";
const char* ap_pass = "12345678";

void startAP() {
  DEBUG2_PRINTLN("Starting Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  IPAddress ip = WiFi.softAPIP();
  DEBUG2_PRINT("AP SSID: ");
  DEBUG2_PRINTLN(ap_ssid);
  DEBUG2_PRINT("AP IP: ");
  DEBUG2_PRINTLN(ip);
}
