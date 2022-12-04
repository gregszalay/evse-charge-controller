#include "config.h"
#include "ControlPilot.h"

void LEDS_CP_STATE_A()
{
    digitalWrite(CP_STATUS_A_PIN, LOW);
    digitalWrite(CP_STATUS_B_PIN, HIGH);
    digitalWrite(CP_STATUS_C_PIN, HIGH);
    digitalWrite(EVSE_STATUS_ERR_PIN, HIGH);
}
void LEDS_CP_STATE_B()
{
    digitalWrite(CP_STATUS_A_PIN, HIGH);
    digitalWrite(CP_STATUS_B_PIN, LOW);
    digitalWrite(CP_STATUS_C_PIN, HIGH);
    digitalWrite(EVSE_STATUS_ERR_PIN, HIGH);
}
void LEDS_CP_STATE_C()
{
    digitalWrite(CP_STATUS_A_PIN, HIGH);
    digitalWrite(CP_STATUS_B_PIN, HIGH);
    digitalWrite(CP_STATUS_C_PIN, LOW);
    digitalWrite(EVSE_STATUS_ERR_PIN, HIGH);
}
void LEDS_CP_STATE_D()
{
    digitalWrite(CP_STATUS_A_PIN, HIGH);
    digitalWrite(CP_STATUS_B_PIN, HIGH);
    digitalWrite(CP_STATUS_C_PIN, LOW);
    digitalWrite(EVSE_STATUS_ERR_PIN, HIGH);
}
void LEDS_CP_STATE_E()
{
    digitalWrite(CP_STATUS_A_PIN, HIGH);
    digitalWrite(CP_STATUS_B_PIN, HIGH);
    digitalWrite(CP_STATUS_C_PIN, HIGH);
    digitalWrite(EVSE_STATUS_ERR_PIN, LOW);
}
void LEDS_CP_STATE_F()
{
    digitalWrite(CP_STATUS_A_PIN, HIGH);
    digitalWrite(CP_STATUS_B_PIN, HIGH);
    digitalWrite(CP_STATUS_C_PIN, HIGH);
    digitalWrite(EVSE_STATUS_ERR_PIN, LOW);
}

void ControlPilot::loop()
{
    CP_LEVELS max = voltage_reader.getMaxRange();
    CP_LEVELS max_prev = voltage_reader.getMaxRangePrev();
    CP_LEVELS min = voltage_reader.getMinRange();
    CP_LEVELS min_prev = voltage_reader.getMinRangePrev();

    // Check diode presence: CP minimum voltage should be -12V
    if (min != CP_LEVELS::RANGE_neg_12_V && generator.getDutyCyclePercent() != 100 && generator.getDutyCyclePercent() != 0)
    {
        Serial.println("Negative voltage error. Diode not present in vehicle.");
        digitalWrite(EVSE_STATUS_ERR_PIN, HIGH);
    }

    if (generator.getDutyCyclePercent() == 0 && max_prev != CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_0_V && min == CP_LEVELS::RANGE_neg_12_V)
    {
        Serial.println("X => F state transition");
        LEDS_CP_STATE_F();
    }

    // Check positive voltages and trigger callbacks on state transitions

    if (max_prev == CP_LEVELS::RANGE_12_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("A => B state transition!");
        LEDS_CP_STATE_B();
        (callbacks.A_B)();
    }
    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_12_V)
    {
        Serial.println("B => A state transition!");
        LEDS_CP_STATE_A();
        (callbacks.B_A)();
    }

    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_6_V)
    {
        Serial.println("B => C state transition!");
        LEDS_CP_STATE_C();
        (callbacks.B_C)();
    }
    else if (max_prev == CP_LEVELS::RANGE_6_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("C => B state transition!");
        LEDS_CP_STATE_B();
        (callbacks.C_B)();
    }

    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_3_V)
    {
        Serial.println("B => D state transition!");
        LEDS_CP_STATE_D();
        (callbacks.B_C)();
    }
    else if (max_prev == CP_LEVELS::RANGE_3_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("D => B state transition!");
        LEDS_CP_STATE_B();
        (callbacks.B_C)();
    }

    else if (max_prev == CP_LEVELS::RANGE_12_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("A => E state transition!");
        LEDS_CP_STATE_E();
        (callbacks.B_E)();
    }
    else if (max_prev == CP_LEVELS::RANGE_9_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("B => E state transition!");
        LEDS_CP_STATE_E();
        (callbacks.B_E)();
    }
    else if (max_prev == CP_LEVELS::RANGE_6_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("C => E state transition!");
        LEDS_CP_STATE_E();
        (callbacks.B_E)();
    }
    else if (max_prev == CP_LEVELS::RANGE_3_V && max == CP_LEVELS::RANGE_0_V)
    {
        Serial.println("D => E state transition!");
        LEDS_CP_STATE_E();
        (callbacks.B_E)();
    }

    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_12_V)
    {
        Serial.println("E => A state transition!");
        LEDS_CP_STATE_A();
        (callbacks.E_A)();
    }
    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_9_V)
    {
        Serial.println("E => B state transition!");
        LEDS_CP_STATE_B();
        (callbacks.E_B)();
    }
    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_6_V)
    {
        Serial.println("E => C state transition!");
        LEDS_CP_STATE_C();
        (callbacks.E_C)();
    }
    else if (max_prev == CP_LEVELS::RANGE_0_V && max == CP_LEVELS::RANGE_3_V)
    {
        Serial.println("E => D state transition!");
        LEDS_CP_STATE_D();
        (callbacks.E_D)();
    }
    else if (max_prev != CP_LEVELS::RANGE_12_V && max == CP_LEVELS::RANGE_12_V)
    {
        Serial.println("X => A state transition!");
        LEDS_CP_STATE_A();
    }

    vTaskDelay(10);
}