#pragma once
#include "drow.hpp"
#include "pid.hpp"

extern void handle_mqtt(DRow **drows, const int temp_sensor_count);
extern void mqtt_init(int sensor_count, PLoop *pid);
