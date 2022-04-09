#include "ClockController.h"

#include <time.h>
#include "config.h"
#include "WiFiMulti.h"

ClockController::ClockController() {}

tm ClockController::getDateTime(void) {
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)) {
    return timeinfo;
  }
}

void ClockController::setupTimeZone(void) {
  WiFiMulti wifiMulti;
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
  }
  
  configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  tm t = getDateTime();
}
