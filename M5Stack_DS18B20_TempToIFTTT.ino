#include <M5Stack.h>
#include <time.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "ClockController.h"
ClockController cc;

#include "IFTTTWebhooks.h"
IFTTTWebhooks ift;

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

int lastRunHour = 0;
int lastRunMin = 0;

float celsius = 0.0;
bool switch_state = false;

void sendWebhooks(double temp, String sw) {
  tm t = cc.getDateTime();
  if (t.tm_hour != lastRunHour) {
    // 1時間おきに送信
    lastRunHour = t.tm_hour;
    lastRunMin = t.tm_min;

    ift.sendToIFTTT("water_temp", "水槽温度通知", String(temp), sw);
  }
}

void sendWebhooks_FanSwitch(bool sw) {
  if (switch_state != sw && sw == true) {
    switch_state = sw;
    ift.sendToIFTTT("water_temp", "水温警告", String(celsius), "OFF -> ON");
    ift.sendToIFTTT("fan_switch", "", "", "");
    return;
  }
  if (switch_state != sw && sw == false) {
    switch_state = sw;
    ift.sendToIFTTT("water_temp", "水温警告", String(celsius), "ON -> OFF");
    ift.sendToIFTTT("fan_switch_off", "", "", "");
    return;
  }
}

void clockTask(void * pvParameters) {
  for (;;) {
    tm timeinfo = cc.getDateTime();
    M5.Lcd.fillRect(0, 0, 319, 110, BLACK);
    M5.Lcd.setTextColor(WHITE);

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.println(&timeinfo, "%Y-%m-%d");
    M5.Lcd.setCursor(10, 50);
    M5.Lcd.println(&timeinfo, "     %H:%M");
    M5.Lcd.println();
    delay(30000);
  }
}

void updateTemp(void * pvParameters) {

  //M5.Lcd.clear();
  bool sw = false;
  String swt = "";

  while (true) {
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Device: ");
    DS18B20.begin();
    int count = DS18B20.getDS18Count();
    M5.Lcd.print(count);
    
    if (count > 0) {
      DS18B20.requestTemperatures();
      for (int i = 0; i < count; i++) {
        celsius = DS18B20.getTempCByIndex(i);

        // LCD表示設定
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.fillRect(0, 110, 319, 209, BLACK);
        M5.Lcd.setTextSize(10);
        
        if (celsius >= 28) {
          sw = true;
          M5.Lcd.setTextColor(RED);
        } else if (celsius >= 26) {
          sw = true;
          M5.Lcd.setTextColor(GREEN);
        } else if (celsius >= 24) {
          sw = false;
          M5.Lcd.setTextColor(YELLOW);
        } else {
          sw = false;
          M5.Lcd.setTextColor(WHITE);
        }
        M5.Lcd.setCursor(10, 120);
        M5.Lcd.print("T:");
        M5.Lcd.println(celsius);

        M5.Lcd.setTextColor(WHITE);
        if (sw) {
          swt = "ON";
        } else {
          swt = "OFF";
        }
        M5.Lcd.setCursor(10, 180);
        M5.Lcd.print("SW:");
        M5.Lcd.println(swt);

        // Webhooksへ送信
        sendWebhooks_FanSwitch(sw);
        sendWebhooks(celsius, swt);

      }
    }
    delay(2000);
  }
}

void setup() {
  M5.begin();
  Wire.begin();
  cc = ClockController();
  cc.setupTimeZone();
  ift = IFTTTWebhooks();

  xTaskCreatePinnedToCore(clockTask, "clockTask", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(updateTemp, "updateTemp", 4096, NULL, 2, NULL, 0);
}

void loop() {
  M5.update();
}
