#pragma once
#include <Arduino.h>
#include <functional>

typedef struct ControlPilotSettings
{
    uint32_t PWM_FREQ;
    uint8_t PWM_DEFAULT_DUTY_CYCLE;
    uint8_t PWM_RESOLUTION;
    uint8_t PWM_CHANNEL;
    uint8_t PWM_OUT_PIN;
    uint8_t PWM_OUT_PIN_2;
    uint8_t CP_READ_EXT_TRIG_PIN;
    uint8_t CONTACTOR_PIN;
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
    std::function<void(void)> A_B = []()
    { Serial.println("A_B not overriden"); };
    std::function<void(void)> B_C = []()
    { Serial.println("B_C not overriden"); };
    std::function<void(void)> B_D = []()
    { Serial.println("B_D not overriden"); };
    std::function<void(void)> C_B = []()
    { Serial.println("C_B not overriden"); };
    std::function<void(void)> D_B = []()
    { Serial.println("D_B not overriden"); };
    std::function<void(void)> A_E = []()
    { Serial.println("A_E not overriden"); };
    std::function<void(void)> B_E = []()
    { Serial.println("B_E not overriden"); };
    std::function<void(void)> C_E = []()
    { Serial.println("C_E not overriden"); };
    std::function<void(void)> D_E = []()
    { Serial.println("D_E not overriden"); };
    std::function<void(void)> A_F = []()
    { Serial.println("A_F not overriden"); };
    std::function<void(void)> B_F = []()
    { Serial.println("B_F not overriden"); };
    std::function<void(void)> C_F = []()
    { Serial.println("C_F not overriden"); };
    std::function<void(void)> D_F = []()
    { Serial.println("D_F not overriden"); };
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

class Loopable
{
public:
    virtual void loop() {}
};

static void CP_TASK(void *param)
{
    while (1)
    {
        Loopable *loopable = (Loopable *)param;
        loopable->loop();
        delay(7);
        taskYIELD();
    }
}

class ControlPilot : public Loopable
{
public:
    CPValues cp_values;
    static volatile uint16_t cp_pos_raw;
    static volatile uint16_t cp_neg_raw;
    ControlPilotSettings_t cp_settings;
    CPCallbacks cp_cbs;

    static volatile uint64_t iter;
    static void IRAM_ATTR CP_POS_ADC_ISR()
    {
        cp_pos_raw = analogRead(36);
        iter++;
    }
    // static void IRAM_ATTR CP_NEG_ADC_ISR()
    // {
    //     cp_neg_raw = analogRead(32);
    //     iter++;
    // }

    ControlPilot(ControlPilotSettings_t _cp_settings, CPCallbacks _cp_cbs) : cp_settings(_cp_settings), cp_cbs(_cp_cbs)
    {

        delay(100);
        pinMode(cp_settings.CP_READ_EXT_TRIG_PIN, INPUT_PULLDOWN);

        delay(100);
        ledcSetup(cp_settings.PWM_CHANNEL, cp_settings.PWM_FREQ, cp_settings.PWM_RESOLUTION);
        ledcAttachPin(cp_settings.PWM_OUT_PIN, cp_settings.PWM_CHANNEL);
        ledcAttachPin(cp_settings.PWM_OUT_PIN_2, cp_settings.PWM_CHANNEL);
        ledcWrite(cp_settings.PWM_CHANNEL, cp_settings.PWM_DEFAULT_DUTY_CYCLE);

        delay(100);
        attachInterrupt(cp_settings.CP_READ_EXT_TRIG_PIN, this->CP_POS_ADC_ISR, RISING);
        // attachInterrupt(CP_READ_EXT_TRIG_PIN, _CP_NEG_ADC_ISR, RISING);
        xTaskCreate(CP_TASK, "CP Task", 1000, this, 1, NULL);
    };

    void setDutyCycle(uint8_t new_value)
    {
        ledcWrite(0, new_value);
    };

    void loop() override
    {
        if (!Serial)
            Serial.begin(115200);
        using namespace CPClassifier;
        cp_values.setCPPos(cp_pos_raw);
        cp_values.setCPNeg(cp_neg_raw);
        if (_6V(cp_values.getCPPos()) && _9V(cp_values.getCPPosPrev()))
        {
            Serial.println("B_C state transition!");
            this->cp_cbs.B_C();
        }
        if (_9V(cp_values.getCPPos()) && _6V(cp_values.getCPPosPrev()))
        {
            Serial.println("B_C state transition!");
            this->cp_cbs.B_C();
        }
    }
};