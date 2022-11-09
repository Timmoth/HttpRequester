# HttpRequester

Small library that helps make HTTP requests on an ESP32 device.

Originally written to help reduce boiler plate code around making HTTP requests for my IOT projects using a Raspberry Pi Pico w.


### Usage:

------------

- clone the repo to the Arduino libraries directory

`cd C:\Users\<username>\Documents\libraries
`

`git clone https://github.com/Timmoth/HttpRequester.git
`
- Add Arduino pico to your board manager (file->preferences->additional board manager urls) paste the following url

`https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`

- Include library in your project
```c
#include <HttpRequester.h>
```

- Connect to your WiFi network
```c
 WiFi.mode(WIFI_STA);
WiFiMulti.addAP(ssid, password);
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.println("connecting...");
  }
 ```
- make request
```c
  HttpResponse* postResponse = requester->Post("/api/user", "{\"name\":\"Tim\"}", "application/json");
  if (postResponse == NULL || !postResponse->IsSuccess()) {
    return;
  }

  Serial.println("Response: " + postResponse->Body);
 ```

### Tests:

------------

Tests are written using [ElkTest](https://github.com/Timmoth/ElkTest "ElkTester") see project readme for more instructions.

### Upcoming:
- Accomadate HTTP requests
- Investigate validating SSL certs
- Improve documentation
- Add more tests
- Make available to the Arduino IDE Library manager
