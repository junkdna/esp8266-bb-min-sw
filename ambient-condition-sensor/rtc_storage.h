/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */

#ifndef _RTC_STORAGE_H_
#define _RTC_STORAGE_H_

#define MEM_BASE 33

class ambient_condition_sensor;

namespace storage_unit_type {
    enum storage_unit_type {
        TEMPERATURE_0 = 0,
        TEMPERATURE_1,
        TEMPERATURE_2,
        HUMIDITY_0,
        HUMIDITY_1,
        HUMIDITY_2,
        PRESSURE_0,
        PRESSURE_1,
        PRESSURE_2,
        NONE
    };
}

class RTC_storage {
private:
    ambient_condition_sensor *context;
    void read_from_rtc_mem();
    void write_to_rtc_mem();

protected:
    float values[storage_unit_type::NONE];
    uint32_t values_state;

public:
    RTC_storage();
    ~RTC_storage();

    void setup(ambient_condition_sensor* cont);
    void update_value(enum storage_unit_type::storage_unit_type type,
                      float value, float difference = 0.2);
    enum storage_unit_type::storage_unit_type new_values();
    float get_value(enum storage_unit_type::storage_unit_type type);
    const char* get_name(enum storage_unit_type::storage_unit_type type);
};

#endif
// vim: ts=4 sw=4 et cindent
