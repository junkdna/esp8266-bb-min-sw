/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

class ambient_condition_sensor;

class control {
    private:
        ambient_condition_sensor *context;
        bool sampling;
        bool initial_startup;
        char wlan_ssid[100];
        char wlan_pwd[100];
        uint32_t sleep_time;
        char chip_id[20];

        void go_online();
        void load_configuration();
        void ota_prepare();
        void ota_handle();

    protected:

    public:
        control();
        ~control() = default;

        void setup(ambient_condition_sensor *cont);
        void switch_state();
        void loop();
        bool sample();
        bool init_run();
};

#endif
// vim: ts=4 sw=4 et cindent
