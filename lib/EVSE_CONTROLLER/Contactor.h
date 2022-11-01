#pragma once
#include <Arduino.h>

class Contactor
{
public:
    const int CONTACTOR_PIN;
    Contactor(int _CONTACTOR_PIN) : CONTACTOR_PIN(_CONTACTOR_PIN)
    {
        pinMode(CONTACTOR_PIN, OUTPUT);
    }
    void On()
    {
        digitalWrite(CONTACTOR_PIN, HIGH);
    };
    void Off()
    {
        digitalWrite(CONTACTOR_PIN, LOW);
    };
};