#include <ArduinoJson.h>
#include <WiFiMulti.h>
#include "arduino_secrets.h"
#include <HttpRequester.h>

const char* ssid = SECRET_NETWORK_SSID;      // your network SSID
const char* password = SECRET_NETWORK_PASS;  // your network password

WiFiMulti WiFiMulti;
HttpRequester* requester;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  ConnectToWifi();
  requester = new HttpRequester();
  requester->Setup("www.fakestoreapi.com", 443);
}

void loop() {
  ConnectToWifi();
  delay(500);

  HttpResponse* getResponse = requester->Get("/products/1");
  if (getResponse == NULL || !getResponse->IsSuccess()) {
    return;
  }

  Serial.println("Body: " + getResponse->Body);

  DynamicJsonDocument doc(400);
  DeserializationError jsonError = deserializeJson(doc, getResponse->Body);
  if (jsonError) {
    Serial.println("Deserialize failed: " + String(jsonError.c_str()));
    return;
  }

  StaticJsonDocument<200> postBody;
  postBody["name"] = "Tenali Ramakrishna";
  postBody["gender"] = "male";
  postBody["email"] = "tenali.ramakrishna@15ce.com";
  postBody["status"] = "active";

  String postBodyJson;
  serializeJson(postBody, postBodyJson);

  HttpResponse* postResponse = requester->Post("/public/v2/users", postBodyJson, "application/json");
  if (postResponse == NULL || !postResponse->IsSuccess()) {
    return;
  }

  Serial.println("Body: " + postResponse->Body);

  delay(500);
}


bool ConnectToWifi() {
  if (WiFiMulti.run() == WL_CONNECTED) {
    return true;
  }
  // wait for WiFi connection
  Serial.println("Connecting to network: " + String(ssid));
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.println("connecting...");
  }

  Serial.println("Succesfully connected!");
  return true;
}