#pragma once
#include <Arduino.h>
#include <functional>
#include "ControlPilot.h"
#include "ProximityPilot.h"
#include "Contactor.h"
#include "taskify.h"

class EVSE
// TODO: make EVSE class singleton
{
private:
    bool charging_allowed = false;

public:
    int id;
    ControlPilot cp;
    ProximityPilot pp;
    Contactor cont;

    inline void setChargingAllowed(bool isAllowed)
    {
        this->charging_allowed = isAllowed;
        if (!this->charging_allowed && this->cont.getIsOn())
        {
            Serial.println("Switching contactor OFF");
            this->cont.Off();
        }
    }
    inline bool getChargingAllowed() { return this->charging_allowed; }

    EVSE(int id);
};