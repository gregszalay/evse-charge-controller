#pragma once
#include <Arduino.h>
#include <functional>
#include "taskify.h"
#include "pins.h"
#include "CPVoltageReader.h"
#include "CPGenerator.h"

class CPCallbacks
{
public:
    // Normal operation callbacks
    std::function<void()> A_B = [this]() // Vehicle connected
    { Serial.println("A_B not defined!"); };

    std::function<void()> B_A = [this]() // Vehicle disconnected
    { Serial.println("A_B not defined!"); };

    std::function<void()> B_C = [this]() // Vehicle requests charging, closes S2 switch
    { Serial.println("B_C not defined!"); };

    std::function<void()> B_D = [this]() // Vehicle requests charging, closes S2 switch, ventillation req.
    { Serial.println("B_D not defined!"); };

    std::function<void()> C_B = [this]() // Vehicle stops charging, opens S2 switch
    { Serial.println("C_B not defined!"); };

    std::function<void()> D_B = [this]() // Vehicle stops charging, opens S2 switch, ventillation was req.
    { Serial.println("D_B not defined!"); };

    // Error callbacks

    std::function<void()> A_E = [this]()
    { Serial.println("A_E not defined!"); };

    std::function<void()> B_E = [this]()
    { Serial.println("B_E not defined!"); };

    std::function<void()> C_E = [this]()
    { Serial.println("C_E not defined!"); };

    std::function<void()> D_E = [this]()
    { Serial.println("D_E not defined!"); };

    std::function<void()> E_A = [this]()
    { Serial.println("E_A not defined!"); };

    std::function<void()> E_B = [this]()
    { Serial.println("E_B not defined!"); };

    std::function<void()> E_C = [this]()
    { Serial.println("E_C not defined!"); };

    std::function<void()> E_D = [this]()
    { Serial.println("E_D not defined!"); };
};

class ControlPilot
{
    CPGenerator generator;

public:
    CPCallbacks callbacks;
    CPVoltageReader voltage_reader;
    inline void start()
    {
        voltage_reader.start();
        generator.startConstantVoltage();
        taskify("CP Task", 10000, this, 1, NULL);
    };

    void loop();
};