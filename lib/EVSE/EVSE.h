#pragma once
#include <Arduino.h>
#include <functional>
#include "ControlPilot.h"
#include "ProximityPilot.h"
#include "Contactor.h"
#include "taskify.h"
#include <string>
#include <sstream>

class EVSEStatus
{
public:
    uint8_t isEVConnected = 0;
    uint8_t isChargingEnabled = 0;
    uint8_t isCharging = 0;
    uint8_t error = 0;
    std::string toString()
    {
        std::stringstream result;
        result << isEVConnected << "," << isChargingEnabled << "," << isCharging << "," << error;
        return result.str();
    }
};

class EVSE
{
private:
    bool charging_allowed = false;

public:
    ControlPilot cp;
    ProximityPilot pp;
    Contactor cont;
    EVSEStatus current_status;

    EVSE()
    {
        // Make sure the contactor is in an off state to start with
        cont.Off();
        setupCallbacks();
        taskify("EVSE Task", 10000, this, 1, NULL);
    }

    void setupCallbacks();
    inline void start()
    {
        cp.start();
        pp.start();
    }
    inline void setChargingAllowed(bool isAllowed)
    {
        this->charging_allowed = isAllowed;
        current_status.isChargingEnabled = isAllowed ? 1 : 0;
        if (!this->charging_allowed && this->cont.getIsOn())
        {
            Serial.println("Switching contactor OFF");
            this->cont.Off();
        }
    }
    inline bool getChargingAllowed() { return this->charging_allowed; }

    void loop()
    {
        // Debugging
        Serial.printf("CP MAX: %f V -- CP MIN: %f V\n", cp.voltage_reader.getMaxV(), cp.voltage_reader.getMinV());
        Serial.printf("CP MAX RANGE: %d -- CP MIN RANGE: %d \n", cp.voltage_reader.getMaxRange(), cp.voltage_reader.getMinRange());
        Serial.printf("CP MAX ADC COUNT: %llu -- CP MIN ADC COUNT: %llu\n", cp_max_count, cp_min_count);
        Serial.printf("PP: %f V\n", pp.pp_voltage);
        Serial.printf("DETECTED CABLE: %d\n", pp.getDetectedCableType());
        vTaskDelay(500);
    }
};