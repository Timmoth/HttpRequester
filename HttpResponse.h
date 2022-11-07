#ifndef HttpResponse_h
#define HttpResponse_h

#include <ArduinoJson.h>
class HttpResponse {
public:
  String Headers;
  int StatusCode;
  String StatusText;
  String Body;

  void Print() {
    Serial.println("Status: " + String(this->StatusCode) + " " + this->StatusText);
    Serial.println("Headers:");
    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, this->Headers);
    serializeJsonPretty(doc, Serial);
    Serial.println();
    String body = this->Body;
    if (body.length() > 0) {
      Serial.println("Body: ");
      Serial.println(body);
    }
  }
};

#endif