#ifndef WifiManager_h
#define WifiManager_h

#include <WiFi.h>

class WifiManager {
public:
  bool Connect(String ssid, String password);

private:
  String _ssid;
  String _password;
};

#endif