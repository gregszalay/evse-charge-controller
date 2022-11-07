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
    void On()
    {
        digitalWrite(contactorPin, HIGH);
        isOn = true;
    };
    void Off()
    {
        digitalWrite(contactorPin, LOW);
        isOn = false;
    };
    bool getIsOn()
    {
        return isOn;
    }
};