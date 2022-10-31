#pragma once
#include <Arduino.h>

class PPValues
{
public:
    float pp;
};

typedef struct ProximityPilotSettings
{
    uint8_t PP_PIN;
} ProximityPilotSettings_t;

class ProximityPilot
{
};