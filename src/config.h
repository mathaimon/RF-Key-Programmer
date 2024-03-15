#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

const uint8_t rfTransmitPin = 23;
unsigned long rfTransmitDelay = 500;

const char *devName = "RF Key Programmer";

const char *ssid = "RF Key Programmer";
const char *password = "12345678";
IPAddress apIP(4, 3, 2, 1);

// Initialize funtions

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len);

// Start sending RF Key.
void startRfTransmission();

// Notify all WS clients about the current status of RF Transmit
void notifyWsClients();
