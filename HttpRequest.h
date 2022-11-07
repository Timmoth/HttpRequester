#ifndef HttpRequest_h
#define HttpRequest_h
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "HttpResponse.h"

enum HttpMethod {
  GET = 0,
  POST = 1,
  PUT = 2,
  DELETE = 3,
};

const String httpMethods[4] = { "GET", "POST", "PUT", "DELETE" };

class HttpRequest {
public:
  HttpRequest();
  ~HttpRequest();

  bool Connect(String url, uint16_t port, String cert = "");
  HttpResponse* Send(HttpMethod Method, String path, String body = "");

  String Url;
  uint16_t Port;
private:
  WiFiClientSecure httpsClient;
  HttpResponse* GetResponse(WiFiClientSecure httpsClient);
};

#endif