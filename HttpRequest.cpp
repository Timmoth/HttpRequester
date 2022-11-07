#include "HttpRequest.h"
HttpRequest::HttpRequest() {
}

HttpRequest::~HttpRequest() {
  if (httpsClient.connected()) {
    httpsClient.stop();
  }
}


bool HttpRequest::Connect(String url, uint16_t port, String cert) {

  if (cert == "") {
    httpsClient.setInsecure();
  } else {
    httpsClient.setCACert(cert.c_str());
  }

  if (httpsClient.connected()) {
    if (this->Url == url && this->Port == port) {
      return true;
    }
    httpsClient.stop();
  }

  this->Url = url;
  this->Port = port;

  if (!httpsClient.connect(this->Url.c_str(), this->Port)) {
    Serial.println("Failed to connect to webserver");
    return false;
  }

  return true;
}

HttpResponse* HttpRequest::Send(HttpMethod method, String path, String body) {

  if (!httpsClient.connected()) {
    Serial.println("Not connected to webserver");
    return NULL;
  }

  String request = String(httpMethods[method]) + " " + path + " HTTP/1.1\r\n";
  String headers = "Host: " + this->Url + "\r\n" + "Content-Type: application/json\r\n" + "accept: */*\r\n" + "Connection: close\r\n" + "Content-Length: " + body.length() + "\r\n";
  // Making the request.
  httpsClient.print(request + headers + +"\r\n" + body + "\r\n\r\n");
  HttpResponse* httpResponse = GetResponse(httpsClient);

  Serial.println("-----Http request-----");
  Serial.print(request);
  Serial.print(headers);
  if (body.length() > 0) {
    Serial.println("Body: ");
    Serial.println(body);
  }
  Serial.println("-----Http response-----");
  httpResponse->Print();
  Serial.println("------------------");

  return httpResponse;
}

HttpResponse* HttpRequest::GetResponse(WiFiClientSecure httpsClient) {
  HttpResponse* httpResponse = new HttpResponse();

  String headers = "{";
  // Receiving response headers.
  for (int nLine = 1; httpsClient.connected(); nLine++) {
    // Reading headers line by line.
    String line = httpsClient.readStringUntil('\n');
    // If headers end, move on.
    if (line == "\r") break;

    // Parse status and statusText from line 1.
    if (nLine == 1) {
      int status = line.substring(line.indexOf(" ")).substring(0, line.indexOf(" ")).toInt();
      String statusText = line.substring(line.indexOf(String(status)) + 4);
      httpResponse->StatusCode = status;
      httpResponse->StatusText = statusText;
    } else {
      if (nLine != 2) {
        headers += ", ";
      }
      String headerKey = line.substring(0, line.indexOf(": "));
      String headerValue = line.substring(line.indexOf(": ") + 2, line.indexOf("\r"));
      headers += "\"" + headerKey + "\":\"" + headerValue + "\"";
    }
  }

  headers += "}";
  httpResponse->Headers = headers;

  String responseBody = "";
  int chunkIndex = 0;

  // Receiving response body.
  while (httpsClient.available()) {
    String chunkData = httpsClient.readStringUntil('\n');
    if (chunkIndex % 2 != 0) {
      responseBody += chunkData;
    }
    chunkIndex++;
  }

  httpResponse->Body = responseBody;

  return httpResponse;
}