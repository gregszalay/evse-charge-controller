#pragma once
#include <Arduino.h>

class Contactor

{
private:
    bool isOn = false;

public:
    const int CONTACTOR_PIN;
    Contactor(int _CONTACTOR_PIN) : CONTACTOR_PIN(_CONTACTOR_PIN)
    {
        pinMode(CONTACTOR_PIN, OUTPUT);
    }
    void On()
    {
        digitalWrite(CONTACTOR_PIN, HIGH);
        isOn = true;
    };
    void Off()
    {
        digitalWrite(CONTACTOR_PIN, LOW);
        isOn = false;
    };
    bool getIsOn()
    {
        return isOn;
    }
};