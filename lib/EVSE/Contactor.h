#pragma once
#include <Arduino.h>

class Contactor

{
private:
    bool isOn = false;

public:
    const int contactorPin;
    Contactor(int _contactorPin) : contactorPin(_contactorPin)
    {
        pinMode(contactorPin, OUTPUT);
    }
    inline void On()
    {
        digitalWrite(contactorPin, HIGH);
        isOn = true;
    };
    inline void Off()
    {
        digitalWrite(contactorPin, LOW);
        isOn = false;
    };
    inline bool getIsOn()
    {
        return isOn;
    }
};