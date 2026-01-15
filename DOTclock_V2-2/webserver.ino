#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "webpage.h"
#include "rtc_eeprom.h"

ESP8266WebServer server(80);

void sendCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void onOptions() {
  sendCORSHeaders();
  server.send(204);
}

void onGetsensor() {
  sendCORSHeaders();
  DynamicJsonDocument doc(256);

  if (!isnan(g_lastTemp)) doc["temp"] = g_lastTemp;
  if (!isnan(g_lastHum)) doc["hum"] = g_lastHum;
  doc["battery"] = "OK";

  DateTime now = rtc.now();
  char buf[32];
  snprintf(
    buf,
    sizeof(buf),
    "%04d-%02d-%02dT%02d:%02d:%02d",
    now.year(), now.month(), now.day(),
    now.hour(), now.minute(), now.second()
  );
  doc["updated_at"] = buf;

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void onSetTime() {
  sendCORSHeaders();
  String body = server.arg("plain");

  if (body.length() == 0) {
    server.send(400, "application/json", "{\"error\":\"empty body\"}");
    return;
  }

  DynamicJsonDocument doc(256);
  DeserializationError err = deserializeJson(doc, body);

  if (err) {
    server.send(400, "application/json", "{\"error\":\"json parse\"}");
    return;
  }

  const char* ctime = doc["custom_time"];
  const char* cdate = doc["custom_date"];

  if (!ctime || !cdate) {
    server.send(400, "application/json", "{\"error\":\"missing fields\"}");
    return;
  }

  if (setRTCFromStrings(cdate, ctime))
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  else
    server.send(400, "application/json", "{\"error\":\"invalid format\"}");
}

void onSetAlarms() {
  sendCORSHeaders();
  String body = server.arg("plain");

  if (body.length() == 0) {
    server.send(400, "application/json", "{\"error\":\"empty body\"}");
    return;
  }

  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, body);

  if (err) {
    server.send(400, "application/json", "{\"error\":\"json parse\"}");
    return;
  }

  if (writeJsonToEEPROM(body))
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  else
    server.send(500, "application/json", "{\"error\":\"eeprom write failed\"}");
}

void onSetText() {
  sendCORSHeaders();
  String body = server.arg("plain");
  if (body.length() == 0) {
    server.send(400, "application/json", "{\"error\":\"empty body\"}");
    return;
  }

  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, body);
  if (err) {
    server.send(400, "application/json", "{\"error\":\"json parse\"}");
    return;
  }

  const char* ct = doc["custom_text"];
  String payloadText = ct ? String(ct) : String("");

  if (payloadText.length() > (sizeof(teksBuf) - 1)) {
    payloadText = payloadText.substring(0, sizeof(teksBuf) - 1);
  }

  DynamicJsonDocument outDoc(512);
  outDoc["custom_text"] = payloadText;

  DateTime now = rtc.now();
  char buf[32];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
           now.year(), now.month(), now.day(),
           now.hour(), now.minute(), now.second());
  outDoc["updated_at"] = buf;

  String outStr;
  serializeJson(outDoc, outStr);

  if (writeJsonToEEPROM(outStr)) {
    strncpy(teksBuf, payloadText.c_str(), sizeof(teksBuf) - 1);
    teksBuf[sizeof(teksBuf) - 1] = '\0';
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    server.send(500, "application/json", "{\"error\":\"eeprom write failed\"}");
  }
}

void initServer() {
  server.on("/", HTTP_GET, []() { server.send_P(200, "text/html", INDEX_HTML); });
  server.on("/styles.css", HTTP_GET, []() { server.send_P(200, "text/css", STYLES_CSS); });
  server.on("/suhu.js", HTTP_GET, []() { server.send_P(200, "application/javascript", SUHU_JS); });
  server.on("/app.js", HTTP_GET, []() { server.send_P(200, "application/javascript", APP_JS); });

  server.on("/getsensor", HTTP_GET, onGetsensor);
  server.on("/set_time", HTTP_POST, onSetTime);
  server.on("/set_time", HTTP_OPTIONS, onOptions);
  server.on("/set_alarms", HTTP_POST, onSetAlarms);
  server.on("/set_alarms", HTTP_OPTIONS, onOptions);
  server.on("/set_text", HTTP_POST, onSetText);
  server.on("/set_text", HTTP_OPTIONS, onOptions);


  server.onNotFound([]() {
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.begin();
  DEBUG2_PRINTLN("HTTP server started (PROGMEM)");
}

void handleClientLoop() {
  server.handleClient();
}
