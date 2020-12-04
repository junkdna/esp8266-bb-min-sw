/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */
#include "ambient-condition-sensor.h"
#include "control.h"
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <LittleFS.h>

control::control():
    context(nullptr),
    sampling(true),
    initial_startup(true),
    wlan_ssid{0},
    wlan_pwd{0},
    sleep_time(1000000),
    chip_id{0}
{
    String(ESP.getChipId()).toCharArray(chip_id, sizeof(chip_id));
}

void control::setup(ambient_condition_sensor *cont)
{
    context = cont;

    load_configuration();

    String resetReason = ESP.getResetReason();
    Serial.println(resetReason);
    if(resetReason == "Power On" || resetReason == "External System") {
        // initial startup
        sampling = false;
        go_online();
        ota_prepare();
    } else {
        // wakeup from rtc
        initial_startup = false;
    }
}

void control::switch_state()
{
    if(context->get_storage().new_values() != storage_unit_type::NONE) {
        Serial.println("new data to transmitt, going online");
        sampling = false;
        go_online();
    }
}

void control::loop()
{
    if(initial_startup)
        ota_handle();
    if(!sampling) {
        ESP.deepSleep(sleep_time, WAKE_RF_DISABLED);
    } else {
        if(context->get_storage().new_values() != storage_unit_type::NONE) {
            Serial.println("new data to transmitt wakeup RF");
            ESP.deepSleep(1000000, WAKE_RF_DEFAULT);
        }
        Serial.println("no new data.");
        ESP.deepSleep(sleep_time, WAKE_RF_DISABLED);
    }
}

bool control::sample()
{
    return sampling;
}

void control::go_online()
{
    if (sampling)
        return;

    WiFi.begin(wlan_ssid, wlan_pwd);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("waiting for getting online");
        delay(500);
    }
}

void control::load_configuration()
{
    LittleFS.begin();

    File file = LittleFS.open("/esp_cfg.json", "r");
    if(!file) {
        Serial.println("could not open file");
        return;
    }

    size_t size = file.size();
    std::unique_ptr<char[]> buf (new char[size]);
    file.readBytes(buf.get(), size);

    StaticJsonDocument<2048> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.println("could not load json file");
        return;
    }

    file.close();

    strcpy(wlan_ssid, doc[chip_id]["wlan_ssid"]);
    strcpy(wlan_pwd, doc[chip_id]["wlan_pwd"]);
    sleep_time = doc[chip_id]["sleep_time"];
    sleep_time = sleep_time*1000000;
}

void control::ota_prepare()
{
    ArduinoOTA.setPasswordHash("afd5ce07c54cfb3e78b5a8cad74f8878");
    ArduinoOTA.onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
            } else { // U_FS
            type = "filesystem";
            }

            // NOTE: if updating FS this would be the place to unmount FS using FS.end()
            Serial.println("Start updating " + type);
            });
    ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
            });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            });
    ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
            } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
            } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
            } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
            } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
            }
            });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
}

void control::ota_handle()
{
    ArduinoOTA.handle();
}

bool control::init_run()
{
    return initial_startup;
}
// vim: ts=4 sw=4 et cindent
