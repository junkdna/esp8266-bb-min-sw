/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright 2020 Dominik Laton <dominik.laton@web.de>
 */

#ifndef _AMBIENT_CONDITION_SENSOR_H_
#define _AMBIENT_CONDITION_SENSOR_H_

#include "rtc_storage.h"
#include "control.h"

class ambient_condition_sensor {
protected:
    RTC_storage storage;
    control ctrl;

public:
    RTC_storage &get_storage();
    control &get_ctrl();
    void setup();
    void loop();
};

#endif
// vim: ts=4 sw=4 et cindent
