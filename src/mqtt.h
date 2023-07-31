#pragma once
#include "drow.h"

extern void mqtt_init();
extern void  mqtt_publish();
extern void mqtt_send(DRow &dr);
extern void handle_mqtt();
extern void mqtt_update_presence(bool state);
