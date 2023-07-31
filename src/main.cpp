#include <Arduino.h>
#include <ESPDateTime.h>

#include <ld2410.h>
#include <lvgl.h>

#include "ui.h"
#include "panel.h"
#include "lwifi.h"
#include "mqtt.h"

static DRow dr;

ld2410 radar;

void init_radar() {
  static const int8_t RxPin = 25;
  static const int8_t TxPin = 26;

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
  mqtt_init();
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
static bool first = true;  // makes things initialise


void loop() {
	if (millis() - lastUpdate > 100) {
      SetTimes();
      ta_display();
			lastUpdate = millis();
	}
  handle_mqtt();
  dr.changed = false;
  dr.presence_changed = false;
  if(radar.isConnected() && millis() - lastReading > 500) {
    lastReading = millis();
    if(radar.presenceDetected()) {
      if(radar.stationaryTargetDetected()) {
        dr.set_s(radar.stationaryTargetDistance(), radar.stationaryTargetEnergy());
      }
      if(radar.movingTargetDetected()) {
        dr.set_m(radar.movingTargetDistance(), radar.movingTargetEnergy());
      }
    } else {
      lv_bar_set_value(ui_SDist, 0, LV_ANIM_ON);
      lv_bar_set_value(ui_SEnergy, 0, LV_ANIM_ON);
      lv_bar_set_value(ui_MDist, 0, LV_ANIM_ON);
      lv_bar_set_value(ui_MEnergy, 0, LV_ANIM_ON);
      dr.set_m(0, 0);
      dr.set_s(0, 0);
    }
    if (first || dr.changed) {
      mqtt_send(dr);
      dr.changed = false;
    }
    if (first || dr.presence_changed) {
      mqtt_update_presence(dr.presence);
      dr.presence_changed = true;
    }
    first = false;
  }
	lv_task_handler();
	delay(5);
}

