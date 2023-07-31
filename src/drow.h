#pragma once
#include <ArduinoJson.h>

struct DRow {
  uint16_t moving_distance = 0;
  uint8_t moving_energy = 0;
  uint16_t stationary_distance = 0;
  uint8_t stationary_energy = 0;
  bool changed = false;
  bool presence = false;
  bool presence_changed = false;
  void set_m(uint16_t md, uint8_t me);
  void set_s(uint16_t sd, uint8_t se);
  void build_json_data(StaticJsonDocument<200>& doc);
};

