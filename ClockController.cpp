#include "ClockController.h"

#include <WiFi.h>
#include <time.h>
#include "config.h"

ClockController::ClockController() {}

tm ClockController::getDateTime(void) {
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)) {
    return timeinfo;
  }
}

void ClockController::setupTimeZone(void) {
  Serial.printf("Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" conected!");
  
  configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  tm t = getDateTime();
  
  WiFi.disconnect(true);
}
