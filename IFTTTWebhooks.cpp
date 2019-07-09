#include "IFTTTWebhooks.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "config.h"
#include "WiFiMulti.h"

IFTTTWebhooks::IFTTTWebhooks() {  
}

// Maker WebHook
void IFTTTWebhooks::sendToIFTTT(String eventname, String value1, String value2, String value3) {
  WiFiClient client;
  const char* server = "maker.ifttt.com";
  
  WiFiMulti wifiMulti;
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("conecting to wifi.");
  while (wifiMulti.run() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("wifi connected!");

  Serial.println("connecting to IFTTT Webhooks server...");
  if (!client.connect(server, 80)) {
    Serial.println("Connection failed!");
    return;
  }
  
  Serial.println("Connected!");
  String url = "/trigger/" + eventname + "/with/key/" + WEBHOOKS_KEY + "?value1=" + value1 + "&value2=" + value2 + "&value3=" + value3;
  client.println("GET " + url + " HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  Serial.print("Waiting for response ");

  int count = 0;
  while (!client.available()) {
    delay(50);
    Serial.print(".");
    count++;
    if (count > 20 * 20) {
      Serial.println("(send) failed!");
      return;
    }
  }
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  if (!client.connected()) {
    Serial.println("disconnecting from server.");
    client.stop();
  }
}

