#include <ArduinoJson.h>
#include <ESPDateTime.h>
#include "ui.h"
#include "drow.h"

void DRow::set_m(uint16_t md, uint8_t me) {
  if (moving_distance != md || moving_energy != me) {
    changed = true;
    if (md) {
      if (!presence) {
        presence = true;
        presence_changed = true;
      }
    } else {
      if (presence) {
        presence = false;
        presence_changed = true;
      }
    }
  }
  moving_distance = md;
  moving_energy = me;
  lv_bar_set_value(ui_MDist, md, LV_ANIM_ON);
  lv_bar_set_value(ui_MEnergy, me, LV_ANIM_ON);
}

void DRow::set_s(uint16_t sd, uint8_t se) {
  if (stationary_distance != sd || stationary_energy != se) {
    changed = true;
    if (sd) {
      if (!presence) {
        presence = true;
        presence_changed = true;
      }
    } else {
      if (presence) {
        presence = false;
        presence_changed = true;
      }
    }
  }
  stationary_distance = sd;
  stationary_energy = se;
  lv_bar_set_value(ui_SDist, sd, LV_ANIM_ON);
  lv_bar_set_value(ui_SEnergy, se, LV_ANIM_ON);
}


void DRow::build_json_data(StaticJsonDocument<200>& doc) {
  if (!(moving_distance || moving_energy  || stationary_distance || stationary_energy)) {
      doc["m_distance"] = 0;
      doc["m_energy"] = 0;
      doc["s_distance"] = 0;
      doc["s_energy"] = 0;
  } else {
    if (moving_distance)
      doc["m_distance"] = moving_distance;
    if (moving_energy)
      doc["m_energy"] = moving_energy;
    if (stationary_distance)
      doc["s_distance"] = stationary_distance;
    if (stationary_energy) 
      doc["s_energy"] = stationary_energy;
  }
  doc["time"] = DateTime.toISOString();
}
