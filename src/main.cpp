#include <Arduino.h>
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include "captive_handler.h"
#include "config.h"
#include "webpage.h"

DNSServer dnsServer;
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting RF Key Programmer");

  // Connect to Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  Serial.print("Current IP : ");
  Serial.println(WiFi.softAPIP());

  dnsServer.setTTL(300);
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [&](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* response = request->beginResponse_P(
        200, "text/html", webpage_html, gzip_html_length);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.begin();
}

void loop() { dnsServer.processNextRequest(); }