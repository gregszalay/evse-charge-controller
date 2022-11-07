#pragma once
#include <Arduino.h>

class PPValues
{
public:
    float pp;
};

typedef struct ProximityPilotSettings
{
    uint8_t ppPin;
} ProximityPilotSettings_t;

class ProximityPilot
{
};