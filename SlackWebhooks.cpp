#include "SlackWebhooks.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include "config.h"
#include "WiFiMulti.h"

SlackWebhooks::SlackWebhooks() {
}

void SlackWebhooks::sendToSlack(String text) {
  HTTPClient http;

  WiFiMulti wifiMulti;
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  // Serial.print("conecting to wifi.");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("wifi connected!");

  // POST TO Slack Incoming Webhooks
  String json = "{\"text\":\"" + text + "\"}";
  // Serial.println(json);

  http.begin(WEBHOOKS_HOST, 443, WEBHOOKS_TRIGGER_PATH, SLACK_ROOT_CA);
  http.addHeader("Content-Type", "application/json" );
  http.POST((uint8_t*)json.c_str(), strlen(json.c_str()));

  // 送信成功かどうかのチェックはしない
  // Serial.println("Send Complete. : Slack Incoming WebHooks");

}
