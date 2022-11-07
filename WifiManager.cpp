#include "WifiManager.h"

bool WifiManager::Connect(String ssid, String password) {
  if (ssid == "" || password == "") {
    // Invalid credentials
    Serial.println("Could not connect to network: Invalid credentials");
    return false;
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (_ssid == ssid && _password == password) {
      // Already connected to specified networks
      return true;
    }
  }
  
  // Disconnect from currently connected network
  WiFi.disconnect();

  _ssid = ssid;
  _password = password;

  Serial.println("Connecting to network '" + _ssid + "'");

  // wait for connection
  unsigned long timeOutPeriod = 5000;
  unsigned long sleepDuration = 750;

  for (int i = 0; i < timeOutPeriod; i+=sleepDuration) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Successfully connected to network!");
      return true;
    }

    WiFi.begin(ssid.c_str(), password.c_str());

    delay(sleepDuration);
  }

  Serial.println("Failed to connect to network");
  return false;
}