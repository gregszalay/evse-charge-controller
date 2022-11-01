#pragma once
#include <Arduino.h>
#include <functional>
#include "interrupts.h"
#include "taskify.h"

typedef struct ControlPilotSettings
{
    uint32_t PWM_FREQ;
    uint8_t PWM_DEFAULT_DUTY_CYCLE;
    uint8_t PWM_RESOLUTION;
    uint8_t PWM_CHANNEL;
    uint8_t PWM_OUT_PIN;
    uint8_t PWM_OUT_PIN_2;
    uint8_t CP_READ_EXT_TRIG_PIN;
    uint8_t CP_POS_ADC_PIN;
    uint8_t CP_NEG_ADC_PIN;
} ControlPilotSettings_t;

namespace CPClassifier
{
    const float _12V_TOP = 0.0;
    const float _12V_BOTTOM = 0.0;
    const float _9V_TOP = 3.3;
    const float _9V_BOTTOM = 3.0;
    const float _6V_TOP = 2.9;
    const float _6V_BOTTOM = 1.0;
    const float _3V_TOP = 0.0;
    const float _3V_BOTTOM = 0.0;
    const float _0V_TOP = 0.0;
    const float _0V_BOTTOM = 0.0;
    const float _neg_12V_TOP = 0.0;
    const float _neg_12V_BOTTOM = 0.0;

    bool _12V(float cp_voltage);
    bool _9V(float cp_voltage);
    bool _6V(float cp_voltage);
    bool _3V(float cp_voltage);
    bool _0V(float cp_voltage);
    bool _neg_12V(float cp_voltage);

} // CPClassifier;

class CPCallbacks
{
public:
};

class CPValues
{
    float cp_pos;
    float cp_pos_prev;
    float cp_neg;
    float cp_neg_prev;

public:
    void setCPPos(uint16_t new_cp_pos)
    {
        float new_value = (float)((3.3 / 4096) * new_cp_pos);
        this->cp_pos_prev = this->cp_pos;
        this->cp_pos = new_value;
    }
    void setCPNeg(uint16_t new_cp_neg)
    {
        float new_value = (float)((3.3 / 4096) * new_cp_neg);
        this->cp_neg_prev = this->cp_neg;
        this->cp_neg = new_value;
    }
    float getCPPos()
    {
        return this->cp_pos;
    }
    float getCPNeg()
    {
        return this->cp_neg;
    }
    float getCPPosPrev()
    {
        return this->cp_pos_prev;
    }
    float getCPNegPrev()
    {
        return this->cp_neg_prev;
    }
};

class ControlPilot
{
public:
    CPValues cp_values;
    ControlPilotSettings_t cp_settings;
    CPCallbacks *cp_cbs = nullptr;
    uint64_t iter_debug;
    std::function<void()> A_B = [this]()
    { Serial.println("A_B not overriden!"); };
    std::function<void()> B_C = [this]()
    { Serial.println("B_C not overriden!"); };
    std::function<void()> B_D = [this]()
    { Serial.println("B_D not overriden!"); };
    std::function<void()> C_B = [this]()
    { Serial.println("C_B not overriden!"); };
    std::function<void()> D_B = [this]()
    { Serial.println("D_B not overriden!"); };
    std::function<void()> A_E = [this]()
    { Serial.println("A_E not overriden!"); };
    std::function<void()> B_E = [this]()
    { Serial.println("B_E not overriden!"); };
    std::function<void()> C_E = [this]()
    { Serial.println("C_E not overriden!"); };
    std::function<void()> D_E = [this]()
    { Serial.println("D_E not overriden!"); };
    std::function<void()> A_F = [this]()
    { Serial.println("A_F not overriden!"); };
    std::function<void()> B_F = [this]()
    { Serial.println("B_F not overriden!"); };
    std::function<void()> C_F = [this]()
    { Serial.println("C_F not overriden!"); };
    std::function<void()> D_F = [this]()
    { Serial.println("D_F not overriden!"); };

    ControlPilot(ControlPilotSettings_t _cp_settings) : cp_settings(_cp_settings){};

    void setup()
    {
        delay(100);
        pinMode(cp_settings.CP_READ_EXT_TRIG_PIN, INPUT_PULLDOWN);

        delay(100);
        ledcSetup(cp_settings.PWM_CHANNEL, cp_settings.PWM_FREQ, cp_settings.PWM_RESOLUTION);
        ledcAttachPin(cp_settings.PWM_OUT_PIN, cp_settings.PWM_CHANNEL);
        ledcAttachPin(cp_settings.PWM_OUT_PIN_2, cp_settings.PWM_CHANNEL);
        ledcWrite(cp_settings.PWM_CHANNEL, cp_settings.PWM_DEFAULT_DUTY_CYCLE);

        delay(100);
        attachInterrupt(cp_settings.CP_READ_EXT_TRIG_PIN, CP_POS_ADC_ISR, RISING);
        // attachInterrupt(CP_READ_EXT_TRIG_PIN, _CP_NEG_ADC_ISR, RISING);
        taskify("CP Task", 10000, this, 1, NULL);
    };

    void setDutyCycle(uint8_t new_value)
    {
        ledcWrite(cp_settings.PWM_CHANNEL, new_value);
    };

    void loop()
    {
        this->iter_debug = iter;
        using namespace CPClassifier;
        cp_values.setCPPos(cp_pos_raw);
        cp_values.setCPNeg(cp_neg_raw);
        if (_6V(cp_values.getCPPos()) && _9V(cp_values.getCPPosPrev()))
        {
            Serial.println("B_C state transition!");
            (this->B_C)();
        }
        if (_9V(cp_values.getCPPos()) && _6V(cp_values.getCPPosPrev()))
        {
            Serial.println("C_B state transition!");
            (this->C_B)();
        }
        vTaskDelay(1);
    }
};