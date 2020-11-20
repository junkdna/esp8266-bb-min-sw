/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */
#include "ambient-condition-sensor.h"
#include "cert.h"
#include "rtc_storage.h"
#include "uploader.h"
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>

#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

uploader::uploader():
    context(nullptr),
    influxdb_user{0},
    influxdb_pwd{0},
    influxdb_db{0},
    influxdb_url{0},
    device_name{0},
    chip_id{0},
    influx(),
    sensor("esp")
{
    String(ESP.getChipId()).toCharArray(chip_id, sizeof(chip_id));
}

void uploader::setup(ambient_condition_sensor *cont)
{
    context = cont;
}

void uploader::loop()
{
    load_configuration();
    // Data point
    configTzTime("PST8PDT", "pool.ntp.org", "time.nis.gov");
    influx.setConnectionParamsV1(influxdb_url, influxdb_db, influxdb_user, influxdb_pwd, InfluxDbCloud2CACert);
    // Check server connection
    if (influx.validateConnection()) {
        Serial.print("Connected to InfluxDB: ");
        Serial.println(influx.getServerUrl());
    } else {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(influx.getLastErrorMessage());
    }
    Serial.print("device name: ");
    Serial.println(device_name);
    sensor.addTag("device", device_name);

    // Store measured value into point
    sensor.clearFields();
}

void uploader::process(enum storage_unit_type::storage_unit_type type)
{
    sensor.addField(context->get_storage().get_name(type), context->get_storage().get_value(type));
}

void uploader::send()
{
    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());
    // Write point
    if (!influx.writePoint(sensor)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(influx.getLastErrorMessage());
    }
}

void uploader::load_configuration()
{
    LittleFS.begin();
    File file = LittleFS.open("esp_cfg.json", "r");
    if(!file)
        return;

    size_t size = file.size();
    std::unique_ptr<char[]> buf (new char[size]);
    file.readBytes(buf.get(), size);

    StaticJsonDocument<2048> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error)
        return;

    file.close();

    Serial.print("device name: ");
    Serial.println(device_name);
    strcpy(influxdb_user,doc[chip_id]["influxdb_user"]);
    strcpy(influxdb_pwd, doc[chip_id]["influxdb_pwd"]);
    strcpy(influxdb_db, doc[chip_id]["influxdb_db"]);
    strcpy(influxdb_url, doc[chip_id]["influxdb_url"]);
    strcpy(device_name, doc[chip_id]["device_name"]);
    Serial.print("device name: ");
    Serial.println(device_name);
}
// vim: ts=4 sw=4 et cindent
