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
    uint8_t isEVConnected = 0;
    uint8_t isChargingEnabled = 0;
    uint8_t isCharging = 0;
    uint8_t error = 0;
    int32_t power = 0;
    int32_t energy = 0;

public:
    uint8_t getIsEVConnected()
    {
        return isEVConnected;
    }
    uint8_t getIsChargingEnabled()
    {
        return isChargingEnabled;
    }
    uint8_t getIsCharging()
    {
        return isCharging;
    }
    uint8_t getError()
    {
        return error;
    }
    int32_t getPower()
    {
        return power;
    }
    int32_t getEnergy()
    {
        return energy;
    }
    friend class EVSE;
};

class EVSE
{
    bool charging_allowed = false;
    ControlPilot cp;
    ProximityPilot pp;
    Contactor cont;
    EVSEStatus status;

public:
    EVSE()
    {
        // Make sure the contactor is in an off state to start with
        cont.Off();
        setupCallbacks();

        pinMode(CP_STATUS_A_PIN, OUTPUT);
        digitalWrite(CP_STATUS_A_PIN, LOW);

        pinMode(CP_STATUS_B_PIN, OUTPUT);
        digitalWrite(CP_STATUS_B_PIN, HIGH);

        pinMode(CP_STATUS_C_PIN, OUTPUT);
        digitalWrite(CP_STATUS_C_PIN, HIGH);

        pinMode(CONTACTOR_STATUS_PIN, OUTPUT);
        digitalWrite(CONTACTOR_STATUS_PIN, HIGH);

        pinMode(EVSE_STATUS_ERR_PIN, OUTPUT);
        digitalWrite(EVSE_STATUS_ERR_PIN, HIGH);
    }

    void setupCallbacks();
    inline void start()
    {
        cp.start();
        pp.start();
        taskify("EVSE Task", 10000, this, 1, NULL);
    }

    inline EVSEStatus getStatus()
    {
        return this->status;
    }

    inline void setChargingAllowed(bool isAllowed)
    {
        this->charging_allowed = isAllowed;
        status.isChargingEnabled = isAllowed ? 1 : 0;
        if (!this->charging_allowed && this->cont.getIsOn())
        {
            Serial.println("Switching contactor OFF");
            this->cont.Off();
            status.isCharging = 0;
        }
    }
    inline bool getChargingAllowed() { return this->charging_allowed; }

    inline void simulateMeterValues()
    {
        if (status.isCharging)
        {
            status.power = 22000;                     // default is 22kW
            status.energy += status.power / 3600 / 2; // assuming that this function is called every 500ms
            digitalWrite(CONTACTOR_STATUS_PIN, LOW);
        }
        else
        {
            digitalWrite(CONTACTOR_STATUS_PIN, HIGH);
            status.power = 0;
        }
    }

    void loop()
    {
        // Debugging
        Serial.printf("ESP32 free HEAP: %uB, free PSRAM: %uB\n", ESP.getFreeHeap(), ESP.getFreePsram());
        Serial.printf("CP MAX: %f V -- CP MIN: %f V\n", cp.voltage_reader.getMaxV(), cp.voltage_reader.getMinV());
        Serial.printf("CP MAX RANGE: %d -- CP MIN RANGE: %d \n", cp.voltage_reader.getMaxRange(), cp.voltage_reader.getMinRange());
        Serial.printf("CP MAX ADC COUNT: %llu -- CP MIN ADC COUNT: %llu\n", cp_max_count, cp_min_count);
        Serial.printf("PP: %f V\n", pp.pp_voltage);
        Serial.printf("DETECTED CABLE: %d\n", pp.getDetectedCableType());
        simulateMeterValues();
        Serial.printf("CHARGE POWER: %u kW\n", status.power);
        Serial.printf("TOTAL CHARGE ENERGY: %u kWh\n", status.energy);
        vTaskDelay(500);
    }
};