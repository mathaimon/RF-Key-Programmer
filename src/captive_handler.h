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
    AsyncWebServerResponse *response = request->beginResponse_P(
        200, "text/html", webpage_html, gzip_html_length);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  }
};
