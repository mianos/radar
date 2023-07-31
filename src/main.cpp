#include <Arduino.h>
#include <ESPDateTime.h>

#include <ld2410.h>
#include <lvgl.h>

#include "ui.h"
#include "panel.h"
#include "lwifi.h"
#include "mqtt.h"


ld2410 radar;
static const int8_t RxPin = 25;
static const int8_t TxPin = 26;

void init_radar() {
  Serial1.begin(256000, SERIAL_8N1, RxPin, TxPin); //UART for monitoring the radar
  if(radar.begin(Serial1)) {
    taf("radar OK");
  } else {
    taf("Radar not connected");
  }
}

void setup()
{
  Serial.begin(115200);
  initLVGL();
  DateTime.setTimeZone("AEST-10AEDT,M10.1.0,M4.1.0/3");
  tzset();
  wifi_connect();
  init_radar();
}

void SetTimes() {
	auto minutes = millis() / 60000; // Convert milliseconds to minutes
	auto seconds = (millis() % 60000) / 1000; // Calculate the remainder seconds

	char buffer[20];
	sprintf(buffer, "%02d:%02d", minutes, seconds);
	lv_label_set_text_fmt(ui_elapsed, buffer);
	lv_label_set_text_fmt(ui_clock, DateTime.format(DateFormatter::TIME_ONLY).c_str());
}

static unsigned long lastUpdate;
static uint32_t lastReading = 0;

void loop() {
	if (millis() - lastUpdate > 100) {
      SetTimes();
      ta_display();
			lastUpdate = millis();
	}
  if(radar.isConnected() && millis() - lastReading > 1000) {
    lastReading = millis();
    if(radar.presenceDetected())
    {
      if(radar.stationaryTargetDetected())
      {
        Serial.print(F("Stationary target: "));
        Serial.print(radar.stationaryTargetDistance());
        Serial.print(F("cm energy:"));
        Serial.println(radar.stationaryTargetEnergy());
        lv_bar_set_value(ui_SDist, radar.stationaryTargetDistance(), LV_ANIM_ON);
        lv_bar_set_value(ui_SEnergy, radar.stationaryTargetEnergy(), LV_ANIM_ON);
      }
      if(radar.movingTargetDetected())
      {
        Serial.print(F("Moving target: "));
        Serial.print(radar.movingTargetDistance());
        Serial.print(F("cm energy:"));
        Serial.println(radar.movingTargetEnergy());
        lv_bar_set_value(ui_MDist, radar.movingTargetDistance(), LV_ANIM_ON);
        lv_bar_set_value(ui_MEnergy, radar.movingTargetEnergy(), LV_ANIM_ON);
      }
    }
    else
    {
      Serial.println(F("No target"));
        lv_bar_set_value(ui_SDist, 0, LV_ANIM_ON);
        lv_bar_set_value(ui_SEnergy, 0, LV_ANIM_ON);
        lv_bar_set_value(ui_MDist, 0, LV_ANIM_ON);
        lv_bar_set_value(ui_MEnergy, 0, LV_ANIM_ON);
    }
  }
	lv_task_handler();
	delay(5);
}

