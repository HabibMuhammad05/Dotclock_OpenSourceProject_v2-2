#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include "rtc_eeprom.h"

const char* ota_hostname = "DOTclock";
const char* ota_password = "12345678";

void setupOTA() {
  DEBUG2_PRINTLN("Setting up OTA...");
  ArduinoOTA.setHostname(ota_hostname);
  ArduinoOTA.setPassword(ota_password);

  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    DEBUG2_PRINT("OTA Start (%s)\n", type.c_str());
  });
  ArduinoOTA.onEnd([]() { DEBUG2_PRINTLN("\nOTA End"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    DEBUG2_PRINT("OTA Progress: %u%%\r", (unsigned int)((progress / (float)total) * 100));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DEBUG2_PRINT("OTA Error [%u]: ", error);
    if (error == OTA_AUTH_ERROR) DEBUG2_PRINTLN("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) DEBUG2_PRINTLN("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) DEBUG2_PRINTLN("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) DEBUG2_PRINTLN("Receive Failed");
    else if (error == OTA_END_ERROR) DEBUG2_PRINTLN("End Failed");
  });

  if (MDNS.begin(ota_hostname)) DEBUG2_PRINTLN("MDNS responder started");
  else DEBUG2_PRINTLN("MDNS begin failed (ok in AP mode sometimes)");

  ArduinoOTA.begin();
  DEBUG2_PRINTLN("OTA ready.");
}

void handleOTA() {
  ArduinoOTA.handle();
}
