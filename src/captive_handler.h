#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "webpage.h"

class CaptiveRequestHandler : public AsyncWebHandler {
 public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request) {
    // request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(302);
    response->addHeader(F("Location"), F("http://4.3.2.1"));
    request->send(response);
  }
};
