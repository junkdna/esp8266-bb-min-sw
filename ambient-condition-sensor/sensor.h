/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */

#ifndef _SENSOR_H_
#define _SENSOR_H_

namespace storage_unit_type {
    enum storage_unit_type;
}
class ambient_condition_sensor;

namespace sensor_type {
    enum sensor_type {
        BME_280 = 0,
        LAST
    };
}

namespace sensor_interface {
    enum sensor_interface {
        I2C_0 = 0,
        I2C_1,
        LAST
    };
}

class sensor {
private:
    enum sensor_interface::sensor_interface interface;
    enum sensor_type::sensor_type type;
    enum storage_unit_type::storage_unit_type storage_unit_temp;
    enum storage_unit_type::storage_unit_type storage_unit_hum;
    enum storage_unit_type::storage_unit_type storage_unit_pres;

    void prepare_interface();

protected:
    ambient_condition_sensor *context=nullptr;

public:
    sensor() = default;
    ~sensor() = default;

    void setup(enum sensor_interface::sensor_interface intf,
               enum sensor_type::sensor_type typ, ambient_condition_sensor *cont,
               enum storage_unit_type::storage_unit_type temp=storage_unit_type::TEMPERATURE_0,
               enum storage_unit_type::storage_unit_type hum=storage_unit_type::HUMIDITY_0,
               enum storage_unit_type::storage_unit_type pres=storage_unit_type::PRESSURE_0);
    void loop();
};

#endif
// vim: ts=4 sw=4 et cindent
