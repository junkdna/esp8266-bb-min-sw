/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */
#include "rtc_storage.h"
#include "ambient-condition-sensor.h"
extern "C" {
#include "user_interface.h"
}

RTC_storage::RTC_storage():
    context(nullptr),
    values{0},
    values_state{0}
{
}

RTC_storage::~RTC_storage()
{
    write_to_rtc_mem();
}

void RTC_storage::setup(ambient_condition_sensor* cont)
{
    context = cont;
    if(!context->get_ctrl().init_run()) {
        read_from_rtc_mem();
    } else {
        write_to_rtc_mem();
    }
}

void RTC_storage::update_value(enum storage_unit_type::storage_unit_type type, float value, float difference)
{
    if(abs(value - values[type]) < difference)
        return;

    values[type] = value;
    values_state = values_state | (1 << type);
    write_to_rtc_mem();
    Serial.print("value: ");
    Serial.println(values[type]);
}

void RTC_storage::read_from_rtc_mem()
{
    system_rtc_mem_read(MEM_BASE, values, sizeof(values));
    system_rtc_mem_read(MEM_BASE+sizeof(values), &values_state, sizeof(values_state));
}

void RTC_storage::write_to_rtc_mem()
{
    system_rtc_mem_write(MEM_BASE, values, sizeof(values));
    system_rtc_mem_write(MEM_BASE+sizeof(values), &values_state, sizeof(values_state));
}

enum storage_unit_type::storage_unit_type RTC_storage::new_values()
{
    if(values_state == 0) {
        return storage_unit_type::NONE;
    }

    for(int i=0; i < storage_unit_type::NONE; ++i) {
        if(((1 << i) & values_state) != 0) {
            return storage_unit_type::storage_unit_type(i);
        }
    }
    return storage_unit_type::NONE;
}

float RTC_storage::get_value(enum storage_unit_type::storage_unit_type type)
{
    values_state = (1 << type) ^ values_state;
    write_to_rtc_mem();
    Serial.print("value: ");
    Serial.println(values[type]);
    return values[type];
}

const char* RTC_storage::get_name(enum storage_unit_type::storage_unit_type type)
{
    switch(type) {
        case storage_unit_type::TEMPERATURE_0:
            return "temperature0";
            break;
        case storage_unit_type::TEMPERATURE_1:
            return "temperature1";
            break;
        case storage_unit_type::TEMPERATURE_2:
            return "temperature2";
            break;
        case storage_unit_type::HUMIDITY_0:
            return "humidity0";
            break;
        case storage_unit_type::HUMIDITY_1:
            return "humidity1";
            break;
        case storage_unit_type::HUMIDITY_2:
            return "humidity2";
            break;
        case storage_unit_type::PRESSURE_0:
            return "pressure0";
            break;
        case storage_unit_type::PRESSURE_1:
            return "pressure1";
            break;
        case storage_unit_type::PRESSURE_2:
            return "pressure2";
            break;
        default:
            return "ERROR";
            break;
    }
}
// vim: ts=4 sw=4 et cindent
