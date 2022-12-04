#pragma once
#include <Arduino.h>
#include "config.h"
#include "taskify.h"
#include "pins.h"
#include "interrupts.h"

class PPValues
{
public:
    float pp;
};

enum CABLE
{
    _NO_CABLE = 0,
    _13 = 13,
    _20 = 20,
    _32 = 32,
    _63 = 63,
    _ERROR = -1,
};

extern hw_timer_t *PP_timer;
class ProximityPilot
{
public:
    volatile float pp_voltage = 0;

    void start()
    {
        pinMode(PP_ADC_PIN, INPUT);
        delay(100);
        taskify("PP Measure Task", 10000, this, 1, NULL);

        PP_timer = timerBegin(0, 80, true);
        timerAttachInterrupt(PP_timer, &PP_ADC_ISR, true);
        timerAlarmWrite(PP_timer, 500000, true);
        timerAlarmEnable(PP_timer);
    }

    void loop()
    {
        pp_voltage = (float)((3.3f / 4096.0f) * pp_raw);
    }

    CABLE getDetectedCableType()
    {
        if (pp_voltage > PP_NO_CABLE_MIN || pp_voltage < PP_ERROR_MAX)
            return CABLE::_ERROR;
        else if (pp_voltage < PP_63_MAX)
            return CABLE::_63;
        else if (pp_voltage < PP_32_MAX)
            return CABLE::_32;
        else if (pp_voltage < PP_20_MAX)
            return CABLE::_20;
        else if (pp_voltage < PP_13_MAX)
            return CABLE::_13;
        else
            return CABLE::_ERROR; // Invalid state
    }
};