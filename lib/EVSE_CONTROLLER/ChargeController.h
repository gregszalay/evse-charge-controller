#pragma once
#include <Arduino.h>
#include <functional>
#include "ControlPilot.h"
#include "ProximityPilot.h"
#include "Contactor.h"

ControlPilotSettings_t evse_cp_settings{
    .PWM_FREQ = 1000,
    .PWM_DEFAULT_DUTY_CYCLE = 100,
    .PWM_RESOLUTION = 8,
    .PWM_CHANNEL = 0,
    .PWM_OUT_PIN = 16,
    .PWM_OUT_PIN_2 = 17,
    .CP_READ_EXT_TRIG_PIN = 39,
    .CP_POS_ADC_PIN = 36,
    .CP_NEG_ADC_PIN = 32,
};

ProximityPilotSettings_t evse_pp_settings{
    .PP_PIN = 34,
};

class ChargeController
{
public:
    int id;
    ControlPilot cp;
    ProximityPilot pp;
    Contactor cont;

    ChargeController(int id) : cp(evse_cp_settings), pp(), cont(5)
    {
        cont.Off();
        this->cp.B_C = [this]()
        { Serial.println("Contactor ON"); this->cont.On(); };
        this->cp.C_B = [this]()
        { Serial.println("Contactor OFF"); this->cont.Off(); };
    }
};