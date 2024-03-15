#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <RCSwitch.h>
#include <WiFi.h>

#include "captive_handler.h"
#include "config.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
#include "webpage.h"

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

RCSwitch mySwitch = RCSwitch();

bool enableRfTransmissionFlag = false;
unsigned long rfKey = 0;
unsigned long lastRfTransmitMillis = 0;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
  Serial.begin(115200);
  Serial.print("Starting ");
  Serial.println(devName);

  mySwitch.enableTransmit(rfTransmitPin);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  Serial.print("Current IP : ");
  Serial.println(WiFi.softAPIP());

  //   dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(
        200, "text/html", webpage_html, gzip_html_length);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  ws.cleanupClients();
  if (enableRfTransmissionFlag) startRfTransmission();
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len &&
      info->opcode == WS_TEXT) {
    DynamicJsonDocument recvDoc(1024);
    DeserializationError error = deserializeJson(recvDoc, data);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    // Print the json data
    serializeJsonPretty(recvDoc, Serial);

    if (recvDoc.containsKey("txKey")) rfKey = recvDoc["txKey"];
    if (recvDoc.containsKey("tx")) enableRfTransmissionFlag = recvDoc["tx"];
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                    client->remoteIP().toString().c_str());
      notifyWsClients();
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void startRfTransmission() {
  if (millis() - lastRfTransmitMillis >= rfTransmitDelay) {
    mySwitch.send(rfKey, 24);
    Serial.print("[RF] Starting Transmission : ");
    Serial.println(rfKey);
    notifyWsClients();
  }
}

void notifyWsClients() {
  char buffer[64];
  DynamicJsonDocument txDoc(1024);
  txDoc["currentTxKey"] = rfKey;
  txDoc["txStatus"] = enableRfTransmissionFlag;
  serializeJson(txDoc, buffer);
  ws.textAll(buffer);
  Serial.println("[WS] Notifying Clients");
}
