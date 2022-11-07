#include <ArduinoJson.h>
#include <HttpRequest.h>
#include <WifiManager.h>

const char* ssid = "ssid";      // your network SSID
const char* password = "password";  // your network password
String address = "www.google.com";
int port = 443;

WifiManager* wifiManager;
HttpRequest* request;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");
  wifiManager = new WifiManager();
  request = new HttpRequest();
}

void loop() {
  Serial.println("Loop");

  if (!wifiManager->Connect(ssid, password)) {
    return;
  }

  if (!request->Connect(address, port)) {
    return;
  }

  HttpResponse* response = request->Send(HttpMethod::GET, "/api/test", "{}");

  if (!response) {
    Serial.println("Could not make request");
    return;
  }

  delay(250);
}