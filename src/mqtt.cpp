#include <Arduino.h>
#include <ESPDateTime.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <StringSplitter.h>

#include "panel.h"
#include "ui.h"
#include "drow.h"

WiFiClient espClient;
PubSubClient client(espClient);

static const char *dname = "ldx";
static DRow *dr;


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    ta("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = String(dname) + '-' + String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {

      String cmnd_topic = String("cmnd/") + dname + "/#";
      client.subscribe(cmnd_topic.c_str());

      ta("mqtt connected");
      // Once connected, publish an announcement...

      StaticJsonDocument<200> doc;
      doc["version"] = 1;
      doc["time"] = DateTime.toISOString();
      String status_topic = "tele/" + String(dname) + "/init";
      String output;
      serializeJson(doc, output);
      client.publish(status_topic.c_str(), output.c_str());
    } else {
      taf("failed, rc=%d, sleeping 5 seconds", client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  lv_obj_add_state(ui_mqtton, LV_STATE_CHECKED); 
}


void handle_mqtt() {
  if (!client.connected()) {
    reconnect();
  }
}

const char *mqtt_server = "mqtt2.mianos.com";

void callback(char *topic_str, byte *payload, unsigned int length) {
  //taf("Message arrived, topic '%s'", topic_str);
  auto topic = String(topic_str);

  auto splitter = StringSplitter(topic, '/', 4); //string_to_split, delimiter, limit)
  int itemCount = splitter.getItemCount();
  if (itemCount < 3) {
    taf("Item count less than 3 %d '%s'", itemCount, topic_str);
    return;
  }
#if 0
  for (int i = 0; i < itemCount; i++) {
    String item = splitter.getItemAtIndex(i);
    taf("Item '%s' index %d", item.c_str(), i);
  }
#endif
  if (splitter.getItemAtIndex(0) == "cmnd") {
    DynamicJsonDocument jpl(1024);
    auto err = deserializeJson(jpl, payload, length);
    if (err) {
      taf("deserializeJson() failed: '%s'", err.c_str());
    }
#if 0
    else {
      String output;
      serializeJson(jpl, output);
      taf("payload '%s'", output.c_str());
    }
#endif
#if 0
    auto dest = splitter.getItemAtIndex(2);
    // Serial.printf("dest '%s'\n", dest.c_str());
    if (dest == "temp") {
      if (!jpl.containsKey("number")) {
        ta("emulated temp set does not  not contain a sensor number number");
        return;
      }
      auto number = jpl["number"].as<unsigned int>();
      if (number < 0 || number >= temp_sensor_count) {
        taf("temp no %d invalid", number);
        return;
      }
      if (jpl.containsKey("emulation")) {
        auto emulation = jpl["emulation"].as<bool>();
        temp_sensors[number]->emulationMode(emulation);
        taf("emulation mode %s", emulation ? "on" : "off");
      }
      if (jpl.containsKey("temp")) {
        auto temp = jpl["temp"].as<double>();
        temp_sensors[number]->setEmulatedTemp(temp);
      }
    } else if (dest == "pump") {
      if (!jpl.containsKey("number")) {
        ta("Does not contain a pump number");
        return;
      }
      if (!jpl.containsKey("speed")) {
        ta("Does not contain a pump speed");
        return;
      }
      auto speed = jpl["speed"].as<unsigned int>();
      auto number = jpl["number"].as<unsigned int>();
      set_speed(number, speed);
      if (number == R_MOTOR) {
        ploop->set_output((double)speed);
      }
    } else if (dest == "pid") {
      String output;
      serializeJson(jpl, output);
      Serial.printf("payload '%s'", output.c_str());
      if (ploop->ProcessUpdateJson(jpl)) {
        publish_settings = true;
      }
    } else if (dest = "report") {
        publish_settings = true;
    }
#endif
  }
}


void mqtt_send(DRow &dr) {
  StaticJsonDocument<200> sdoc;
  dr.build_json_data(sdoc);
  String o2;
  serializeJson(sdoc, o2);
  String t2 = String("tele/") + dname + "/radar";
  client.publish(t2.c_str(), o2.c_str());
}


void mqtt_update_presence(bool state) {
  StaticJsonDocument<200> doc;
  doc["state"] = state;
  doc["time"] = DateTime.toISOString();
  String status_topic = "tele/" + String(dname) + "/presence";
  String output;
  serializeJson(doc, output);
  client.publish(status_topic.c_str(), output.c_str());
}

void mqtt_init() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
