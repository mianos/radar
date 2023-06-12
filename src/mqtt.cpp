#include <Arduino.h>
#include <ESPDateTime.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <StringSplitter.h>

#include "drow.hpp"
#include "panel.h"
#include "cservo.hpp"
#include "pid.hpp"

WiFiClient espClient;
PubSubClient client(espClient);
PLoop *ploop;

const char *dname = "stillerator";


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    ta("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "stillerator-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {

      String cmnd_topic = String("cmnd/") + dname + "/#";
      client.subscribe(cmnd_topic.c_str());

      ta("mqtt connected");
      // Once connected, publish an announcement...
#if 1
      StaticJsonDocument<200> doc;
      doc["version"] = 2;
//      doc["run"] = apid.GetMode();
      doc["time"] = DateTime.toISOString();
//      for (auto ii = 0; ii < N_PIDS; ii++) {
//        doc["config"]["servo"] = ii;
//        doc["config"]["sensor"] = input_for_output[ii]; 
//      }
      String status_topic = "tele/" + String(dname) + "/init";
      String output;
      serializeJson(doc, output);
      client.publish(status_topic.c_str(), output.c_str());
//			apid.Publish(client, dname, "Reconnect");
#endif
    } else {
      taf("failed, rc=%d, sleeping 5 seconds", client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void mqtt_send(bool forced, DRow *dr, int snum) {
  StaticJsonDocument<200> doc;
  doc["sensor"] = snum;
  doc["temp"] = dr->temp;
  doc["timestamp"] = dr->temp_set_time;
  if (forced) {
    doc["forced"] = true;
  } else {
    doc["diff"] = dr->diff;
  }
  String output;
  serializeJson(doc, output);
  String tele_topic = String("tele/") + dname + "/temp";
  client.publish(tele_topic.c_str(), output.c_str());
}

time_t *mqtt_sent;

void handle_mqtt(DRow **drows, const int temp_sensor_count) {
  if (!client.connected()) {
    reconnect();
  }
  for (auto snum = 0; snum < temp_sensor_count; snum++) {
    if (drows[snum]->temp <= TempSensor::TooLow || drows[snum]->temp > TempSensor::TooHigh) {
      continue;
    }
    auto time_since_last_mqtt_send = DateTime.now() - mqtt_sent[snum];
    if (time_since_last_mqtt_send > 20) {
      mqtt_send(true, drows[snum], snum);
      mqtt_sent[snum] = DateTime.now();
    } else if (drows[snum]->changed) {
      mqtt_send(false, drows[snum], snum);
      mqtt_sent[snum] = DateTime.now();
    }
  }
  client.loop();
}
 
const char *mqtt_server = "mqtt2.mianos.com";

void callback(char *topic_str, byte *payload, unsigned int length) {
  //taf("Message arrived, topic '%s'", topic_str);
  auto topic = String(topic_str);

  auto splitter = StringSplitter(topic, '/', 4); // new StringSplitter(string_to_split, delimiter, limit)
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
#if 1
    else {
      String output;
      serializeJson(jpl, output);
      taf("payload '%s'", output.c_str());
    }
#endif
    auto dest = splitter.getItemAtIndex(2);
    taf("dest '%s'", dest.c_str());
    if (dest == "pump") {
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
    } else if (dest == "pid") {
      ploop->ProcessUpdateJson(jpl);
    }
  }
#if 0
      else if (dest == "pid") {
      apid.ProcessUpdateJson(jpl);
    } else if (dest == "config") {
      auto config_id = -1;
      if (jpl.containsKey("config")) {
        config_id = jpl["config"].as<int>();
      }
      if (jpl.containsKey("sensor")) {
        Serial.printf("config sensor %d\n", jpl["sensor"].as<int>());    
      }
      if (jpl.containsKey("servo")) {
          Serial.printf("config servo %d\n", jpl["servo"].as<int>());
      }
      if (config_id == -1) {
        Serial.printf("no config id for minmap\n");
        return;
      }
      for (auto ii = 0; ii < num_servos; ii++) {
        if (ii == config_id) {
          if (jpl.containsKey("minmap")) {
            servos[ii]->set_minmap(jpl["minmap"].as<int>());
          }
          if (jpl.containsKey("fullspeed")) {
            servos[ii]->full_speed();
          }
          if (jpl.containsKey("fullstop")) {
            servos[ii]->full_stop();
          }
        }
      }
    }
  }
  apid.Publish(client, dname, "config change");
#endif
}

void mqtt_init(int sensor_count, PLoop *pid) {
  mqtt_sent = new long int[sensor_count];
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  ploop = pid;
}

