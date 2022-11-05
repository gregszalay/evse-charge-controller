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
    extern float _12V_TOP;
    extern float _12V_BOTTOM;
    extern float _9V_TOP;
    extern float _9V_BOTTOM;
    extern float _6V_TOP;
    extern float _6V_BOTTOM;
    extern float _3V_TOP;
    extern float _3V_BOTTOM;
    extern float _0V_TOP;
    extern float _0V_BOTTOM;
    extern float _neg_12V_TOP;
    extern float _neg_12V_BOTTOM;

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
#ifdef CALIBRATE_MODE
        pinMode(27, INPUT_PULLUP);
#endif
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

    // void calibrateInner()
    // {
    //     Serial.println("Waiting for _12V_TOP");
    //     if (!digitalRead(27))
    //         CPClassifier::_12V_TOP

    int counter = 0;
    bool iscalibrated = false;

    struct
    {
        float _12V_TOP_c = -1;
        float _12V_BOTTOM_c = -1;
        float _9V_TOP_c = -1;
        float _9V_BOTTOM_c = -1;
        float _6V_TOP_c = -1;
        float _6V_BOTTOM_c = -1;
        float _3V_TOP_c = -1;
        float _3V_BOTTOM_c = -1;
        float _0V_TOP_c = -1;
        float _0V_BOTTOM_c = -1;
        float _neg_12V_TOP_c = -1;
        float _neg_12V_BOTTOM_c = -1;
    } calibratedvalues;

    void calibrate()
    {
        if (iscalibrated)
            return;
        switch (counter)
        {
        case 0:
            Serial.println("Waiting for _12V_TOP");
            break;
        case 1:
            Serial.println("Waiting for _12V_BOTTOM");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._12V_TOP_c == -1)
                calibratedvalues._12V_TOP_c = cp_values.getCPPos();
            break;
        case 2:
            Serial.println("Waiting for _9V_TOP");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._12V_BOTTOM_c == -1)
                calibratedvalues._12V_BOTTOM_c = cp_values.getCPPos();
            break;
        case 3:
            Serial.println("Waiting for _9V_BOTTOM");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._9V_TOP_c == -1)
                calibratedvalues._9V_TOP_c = cp_values.getCPPos();
            break;
        case 4:
            Serial.println("Waiting for _6V_TOP");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._9V_BOTTOM_c == -1)
                calibratedvalues._9V_BOTTOM_c = cp_values.getCPPos();
            break;
        case 5:
            Serial.println("Waiting for _6V_BOTTOM");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._6V_TOP_c == -1)
                calibratedvalues._6V_TOP_c = cp_values.getCPPos();
            break;
        case 6:
            Serial.println("Waiting for _3V_TOP");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._6V_BOTTOM_c == -1)
                calibratedvalues._6V_BOTTOM_c = cp_values.getCPPos();
            break;
        case 7:
            Serial.println("Waiting for _3V_BOTTOM");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._3V_TOP_c == -1)
                calibratedvalues._3V_TOP_c = cp_values.getCPPos();
            break;
        case 8:
            Serial.println("Waiting for _0V_TOP");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._3V_BOTTOM_c == -1)
                calibratedvalues._3V_BOTTOM_c = cp_values.getCPPos();
            break;
        case 9:
            Serial.println("Waiting for _0V_BOTTOM");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._0V_TOP_c == -1)
                calibratedvalues._0V_TOP_c = cp_values.getCPPos();
            break;
        case 10:
            Serial.println("Finished!");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._0V_BOTTOM_c == -1)
                calibratedvalues._0V_BOTTOM_c = cp_values.getCPPos();
            iscalibrated = true;
            CPClassifier::_12V_TOP = calibratedvalues._12V_TOP_c;
            CPClassifier::_12V_BOTTOM = calibratedvalues._12V_BOTTOM_c;
            CPClassifier::_9V_TOP = calibratedvalues._9V_TOP_c;
            CPClassifier::_9V_BOTTOM = calibratedvalues._9V_BOTTOM_c;
            CPClassifier::_6V_TOP = calibratedvalues._6V_TOP_c;
            CPClassifier::_6V_BOTTOM = calibratedvalues._6V_BOTTOM_c;
            CPClassifier::_3V_TOP = calibratedvalues._3V_TOP_c;
            CPClassifier::_3V_BOTTOM = calibratedvalues._3V_BOTTOM_c;
            CPClassifier::_0V_TOP = calibratedvalues._0V_TOP_c;
            CPClassifier::_0V_BOTTOM = calibratedvalues._0V_BOTTOM_c;
            CPClassifier::_neg_12V_TOP = calibratedvalues._neg_12V_TOP_c;
            CPClassifier::_neg_12V_BOTTOM = calibratedvalues._neg_12V_BOTTOM_c;
            CPClassifier::_12V_TOP;
            Serial.printf("CPClassifier::_12V_TOP: %f\n", CPClassifier::_12V_TOP);
            Serial.printf("CPClassifier::_12V_BOTTOM: %f\n", CPClassifier::_12V_BOTTOM);
            Serial.printf("CPClassifier::_9V_TOP: %f\n", CPClassifier::_9V_TOP);
            Serial.printf("CPClassifier::_9V_BOTTOM: %f\n", CPClassifier::_9V_BOTTOM);
            Serial.printf("CPClassifier::_6V_TOP: %f\n", CPClassifier::_6V_TOP);
            Serial.printf("CPClassifier::_6V_BOTTOM: %f\n", CPClassifier::_6V_BOTTOM);
            Serial.printf("CPClassifier::_3V_TOP: %f\n", CPClassifier::_3V_TOP);
            Serial.printf("CPClassifier::_3V_BOTTOM: %f\n", CPClassifier::_3V_BOTTOM);
            Serial.printf("CPClassifier::_0V_TOP: %f\n", CPClassifier::_0V_TOP);
            Serial.printf("CPClassifier::_0V_BOTTOM: %f\n", CPClassifier::_0V_BOTTOM);
            Serial.printf("CPClassifier::_neg_12V_TOP: %f\n", CPClassifier::_neg_12V_TOP);
            Serial.printf("CPClassifier::_neg_12V_BOTTOM: %f\n", CPClassifier::_neg_12V_BOTTOM);
            break;
        default:
            break;
        }
    }

    void loop()
    {
#ifdef CALIBRATE_MODE
        if (!digitalRead(27))
        {
            while (!digitalRead(27))
            {
                Serial.print(".");
                vTaskDelay(100);
            }
            calibrate();
            counter++;
        }
#endif
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