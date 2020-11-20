/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */

#ifndef _UPLOADER_H_
#define _UPLOADER_H_

#include <InfluxDbClient.h>

namespace storage_unit_type {
    enum storage_unit_type;
}
class ambient_condition_sensor;

class uploader {
    private:
        ambient_condition_sensor *context;
        char influxdb_user[100];
        char influxdb_pwd[100];
        char influxdb_db[100];
        char influxdb_url[100];
        char device_name[100];
        char chip_id[20];
        InfluxDBClient influx;
        Point sensor;

        void load_configuration();

    protected:

    public:
        uploader();
        ~uploader() = default;

        void setup(ambient_condition_sensor *context);
        void loop();
        void process(enum storage_unit_type::storage_unit_type type);
        void send();
};

#endif
// vim: ts=4 sw=4 et cindent
