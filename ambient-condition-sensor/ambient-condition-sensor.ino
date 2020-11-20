/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */
#include "ambient-condition-sensor.h"
#include "sensor.h"
#include "uploader.h"

#define NO_SENSORS 1

ambient_condition_sensor ambient_sensor;
sensor sensors[NO_SENSORS];
uploader upload;

RTC_storage &ambient_condition_sensor::get_storage()
{
    return this->storage;
}

control &ambient_condition_sensor::get_ctrl()
{
    return this->ctrl;
}

void ambient_condition_sensor::setup()
{
}

void ambient_condition_sensor::loop()
{
}

void setup() {
    Serial.begin(115200);

    Serial.println("setup begin.");
    Serial.println(String(ESP.getChipId()));
    ambient_sensor.setup();
    upload.setup(&ambient_sensor);
    ambient_sensor.get_ctrl().setup(&ambient_sensor);
    ambient_sensor.get_storage().setup(&ambient_sensor);
    ambient_sensor.get_ctrl().switch_state();
    sensors[0].setup(sensor_interface::I2C_1, sensor_type::BME_280, &ambient_sensor,
            storage_unit_type::TEMPERATURE_0, storage_unit_type::HUMIDITY_0,
            storage_unit_type::PRESSURE_0);
    Serial.println("setup ended.");
}

void loop() {
    ambient_sensor.loop();

    if(ambient_sensor.get_ctrl().sample()) {
        for(int i=0; i < NO_SENSORS; ++i) {
            sensors[i].loop();
        }
    } else {
        if(!ambient_sensor.get_ctrl().init_run()) {
            upload.loop();
            enum storage_unit_type::storage_unit_type type;
            while(true) {
                type = ambient_sensor.get_storage().new_values();
                if(type == storage_unit_type::NONE)
                    break;
                upload.process(type);
            }
            upload.send();
        }
    }
    ambient_sensor.get_ctrl().loop();
}
// vim: ts=4 sw=4 et cindent
