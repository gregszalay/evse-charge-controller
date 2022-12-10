#pragma once
#include <Arduino.h>
#include "pins.h"

class Contactor
{
private:
    bool isOn = false;

public:
    Contactor()
    {
        // TODO: refine logic for safety control
        pinMode(CONTACTOR_ENABLE_PIN, OUTPUT);
        digitalWrite(CONTACTOR_ENABLE_PIN, HIGH);

        pinMode(CONTACTOR_PIN, OUTPUT);
    }
    inline void On()
    {
        digitalWrite(CONTACTOR_PIN, HIGH);
        isOn = true;
    };
    inline void Off()
    {
        digitalWrite(CONTACTOR_PIN, LOW);
        isOn = false;
    };
    inline bool getIsOn()
    {
        return isOn;
    }
};