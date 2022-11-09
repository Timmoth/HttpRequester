#ifndef HttpRequest_h
#define HttpRequest_h
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "HttpResponse.h"

class HttpRequester
{
public:
  WiFiClientSecure *client;
  HTTPClient https;

  String Host;
  int Port;
  String Path;

  HttpRequester()
  {
    client = new WiFiClientSecure;
  }

  ~HttpRequester()
  {
    https.end();
    delete client;
  }

  bool Setup(String host, int port)
  {
    if (!client)
    {
      Serial.println("Unable to create client");
      return false;
    }

    client->setInsecure();

    Host = host;
    Port = port;
    Path = "/";

    return true;
  }

  HttpResponse *Get(String path)
  {
    return Send("GET", path);
  }

  HttpResponse *Post(String path, String body, String contentType)
  {
    return Send("POST", path, body, contentType);
  }

  HttpResponse *Put(String path, String body, String contentType)
  {
    return Send("PUT", path, body, contentType);
  }

  HttpResponse *Delete(String path)
  {
    return Send("DELETE", path);
  }

private:
  HttpResponse *Send(const char *method, String path)
  {
    if (!Begin(path))
    {
      return NULL;
    }

    int statusCode = https.sendRequest(method);
    Serial.println(String(method) + " " + path + " " + String(statusCode));

    HttpResponse *response = new HttpResponse(statusCode);

    if (response->IsSuccess())
    {
      response->Body = https.getString();
    }

    return response;
  }

  HttpResponse *Send(const char *method, String path, String body, String contentType)
  {
    if (!Begin(path))
    {
      return NULL;
    }

    https.addHeader("Content-Type", contentType, false, true);

    int statusCode = https.sendRequest(method, body);
    Serial.println(String(method) + " " + path + " " + String(statusCode));

    HttpResponse *response = new HttpResponse(statusCode);

    if (response->IsSuccess())
    {
      response->Body = https.getString();
    }

    return response;
  }

  bool Begin(String path)
  {
    if (!client)
    {
      Serial.println("Not connected to server");
      return false;
    }

    if (path != Path || !https.connected())
    {
      Path = path;
      if (!https.begin(*client, Host, Port, Path, true))
      {
        Serial.println("Not connected to server");
        return false;
      }
      
      https.setReuse(true);
    }

    return true;
  }
};
#endif