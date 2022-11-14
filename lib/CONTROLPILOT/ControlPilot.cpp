#include "config.h"
#include "ControlPilot.h"

void ControlPilot::loop()
{
    CP_LEVELS max = voltage_reader.getMaxRange();
    CP_LEVELS max_prev = voltage_reader.getMaxRangePrev();
    CP_LEVELS min = voltage_reader.getMinRange();
    CP_LEVELS min_prev = voltage_reader.getMinRangePrev();

    // Check diode presence: CP minimum voltage should be -12V
    if (min != CP_LEVELS::RANGE_neg_12_V)
    {
        Serial.println("Negative voltage error. Diode not present in vehicle.");
    }

    // Check positive voltages and trigger callbacks on state transitions

    if (max_prev == CP_LEVELS::RANGE_12_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("A => B state transition!");
        (callbacks.A_B)();
    }
    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_12_V)
    {
        Serial.println("B => A state transition!");
        (callbacks.B_A)();
    }

    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_6_V)
    {
        Serial.println("B => C state transition!");
        (callbacks.B_C)();
    }
    else if (max_prev == CP_LEVELS::RANGE_6_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("C => B state transition!");
        (callbacks.C_B)();
    }

    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_3_V)
    {
        Serial.println("B => D state transition!");
        (callbacks.B_C)();
    }
    else if (max_prev == CP_LEVELS::RANGE_3_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("D => B state transition!");
        (callbacks.B_C)();
    }

    else if (max_prev == CP_LEVELS::RANGE_12_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("A => E state transition!");
        (callbacks.B_E)();
    }
    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("B => E state transition!");
        (callbacks.B_E)();
    }
    else if (max_prev == CP_LEVELS::RANGE_6_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("C => E state transition!");
        (callbacks.B_E)();
    }
    else if (max_prev == CP_LEVELS::RANGE_3_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("D => E state transition!");
        (callbacks.B_E)();
    }

    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_12_V)
    {
        Serial.println("E => A state transition!");
        (callbacks.E_A)();
    }
    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("E => B state transition!");
        (callbacks.E_B)();
    }
    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_6_V)
    {
        Serial.println("E => C state transition!");
        (callbacks.E_C)();
    }
    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_3_V)
    {
        Serial.println("E => D state transition!");
        (callbacks.E_D)();
    }

    vTaskDelay(10);
}