#ifndef HttpRequest_h
#define HttpRequest_h
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "HttpResponse.h"

class HttpRequester {
public:
  WiFiClientSecure* client;
  HTTPClient https;

  String Host;
  int Port;
  String Path;

  HttpRequester() {
    client = new WiFiClientSecure;
  }

  ~HttpRequester() {
    https.end();
    delete client;
  }

  bool Setup(String host, int port) {
    if (!client) {
      Serial.println("Unable to create client");
      return false;
    }

    client->setInsecure();

    if (!https.begin(*client, host, port, "/", true)) {
      Serial.println("Unable to connect to server");
      return false;
    }

    Host = host;
    Port = port;
    Path = "/";

    https.setReuse(true);

    return true;
  }

  HttpResponse* Get(String path) {
    if (!client) {
      Serial.println("Not connected to server");
      return NULL;
    }

    if (path != Path || !https.connected()) {
      Path = path;
      if (!https.begin(Host, Port, Path, true)) {
        Serial.println("Not connected to server");
        return NULL;
      }
    }

    int statusCode = https.GET();

    Serial.println("GET " + path + " " + String(statusCode));

    HttpResponse* response = new HttpResponse();

    response->StatusCode = statusCode;

    if (statusCode >= 200 && statusCode <= 299) {
      response->Body = https.getString();
    }

    return response;
  }

  HttpResponse* Post(String path, String body, String contentType) {
    if (!client) {
      Serial.println("Not connected to server");
      return NULL;
    }

    if (path != Path || !https.connected()) {
      Path = path;
      if (!https.begin(Host, Port, Path, true)) {
        Serial.println("Not connected to server");
        return NULL;
      }
    }

    https.addHeader("Content-Type", contentType, false, true);

    int statusCode = https.POST(body);

    Serial.println("POST " + path + " " + String(statusCode));

    HttpResponse* response = new HttpResponse();

    response->StatusCode = statusCode;

    if (statusCode >= 200 && statusCode <= 299) {
      response->Body = https.getString();
    }

    return response;
  }
};
#endif