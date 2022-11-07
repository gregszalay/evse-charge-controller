#pragma once
#include <Arduino.h>
#include <functional>
#include "interrupts.h"
#include "taskify.h"
#include "pins.h"

typedef struct ControlPilotSettings
{
    uint32_t PWM_FREQ;
    uint8_t PWM_DEFAULT_DUTY_CYCLE;
    uint8_t PWM_RESOLUTION;
    uint8_t PWM_CHANNEL;
} ControlPilotSettings_t;

namespace CPClassifier
{
    extern float _13V;
    extern float _11V;
    extern float _10V;
    extern float _8V;
    extern float _7V;
    extern float _5V;
    extern float _4V;
    extern float _2V;
    extern float _1V;
    extern float _neg_1V;
    extern float _neg_13V;
    extern float _neg_11V;

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
    std::function<void()> B_A = [this]()
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
        pinMode(CP_POS_ADC_TRIG_PIN, INPUT_PULLDOWN);

        delay(100);
        ledcSetup(cp_settings.PWM_CHANNEL, cp_settings.PWM_FREQ, cp_settings.PWM_RESOLUTION);
        ledcAttachPin(PWM_PIN, cp_settings.PWM_CHANNEL);
        ledcAttachPin(PWM_PIN_2, cp_settings.PWM_CHANNEL);
        ledcWrite(cp_settings.PWM_CHANNEL, cp_settings.PWM_DEFAULT_DUTY_CYCLE);

        delay(100);
        attachInterrupt(CP_POS_ADC_TRIG_PIN, CP_POS_ADC_ISR, RISING);
        attachInterrupt(CP_NEG_ADC_TRIG_PIN, CP_NEG_ADC_ISR, RISING);
        taskify("CP Task", 10000, this, 1, NULL);
    };

    void setDutyCycle(uint8_t new_value)
    {
        ledcWrite(cp_settings.PWM_CHANNEL, new_value);
    };

    // void calibrateInner()
    // {
    //     Serial.println("Waiting for _13V");
    //     if (!digitalRead(27))
    //         CPClassifier::_13V

    int counter = 0;
    bool iscalibrated = false;

    struct
    {
        float _13V_c = -1;
        float _11V_c = -1;
        float _10V_c = -1;
        float _8V_c = -1;
        float _7V_c = -1;
        float _5V_c = -1;
        float _4V_c = -1;
        float _2V_c = -1;
        float _1V_c = -1;
        float _neg_1V_c = -1;
        float _neg_13V_c = -1;
        float _neg_11V_c = -1;
    } calibratedvalues;

    void calibrate()
    {
        if (iscalibrated)
            return;
        switch (counter)
        {
        case 0:
            Serial.println("Waiting for _13V");
            break;
        case 1:
            Serial.println("Waiting for _11V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._13V_c == -1)
                calibratedvalues._13V_c = cp_values.getCPPos();
            break;
        case 2:
            Serial.println("Waiting for _10V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._11V_c == -1)
                calibratedvalues._11V_c = cp_values.getCPPos();
            break;
        case 3:
            Serial.println("Waiting for _8V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._10V_c == -1)
                calibratedvalues._10V_c = cp_values.getCPPos();
            break;
        case 4:
            Serial.println("Waiting for _7V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._8V_c == -1)
                calibratedvalues._8V_c = cp_values.getCPPos();
            break;
        case 5:
            Serial.println("Waiting for _5V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._7V_c == -1)
                calibratedvalues._7V_c = cp_values.getCPPos();
            break;
        case 6:
            Serial.println("Waiting for _4V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._5V_c == -1)
                calibratedvalues._5V_c = cp_values.getCPPos();
            break;
        case 7:
            Serial.println("Waiting for _2V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._4V_c == -1)
                calibratedvalues._4V_c = cp_values.getCPPos();
            break;
        case 8:
            Serial.println("Waiting for _1V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._2V_c == -1)
                calibratedvalues._2V_c = cp_values.getCPPos();
            break;
        case 9:
            Serial.println("Waiting for _neg_1V");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._1V_c == -1)
                calibratedvalues._1V_c = cp_values.getCPPos();
            break;
        case 10:
            Serial.println("Finished!");
            cp_values.setCPPos(cp_pos_raw);
            if (calibratedvalues._neg_1V_c == -1)
                calibratedvalues._neg_1V_c = cp_values.getCPPos();
            iscalibrated = true;
            CPClassifier::_13V = calibratedvalues._13V_c;
            CPClassifier::_11V = calibratedvalues._11V_c;
            CPClassifier::_10V = calibratedvalues._10V_c;
            CPClassifier::_8V = calibratedvalues._8V_c;
            CPClassifier::_7V = calibratedvalues._7V_c;
            CPClassifier::_5V = calibratedvalues._5V_c;
            CPClassifier::_4V = calibratedvalues._4V_c;
            CPClassifier::_2V = calibratedvalues._2V_c;
            CPClassifier::_1V = calibratedvalues._1V_c;
            CPClassifier::_neg_1V = calibratedvalues._neg_1V_c;
            CPClassifier::_neg_13V = calibratedvalues._neg_13V_c;
            CPClassifier::_neg_11V = calibratedvalues._neg_11V_c;
            CPClassifier::_13V;
            Serial.printf("CPClassifier::_13V: %f\n", CPClassifier::_13V);
            Serial.printf("CPClassifier::_11V: %f\n", CPClassifier::_11V);
            Serial.printf("CPClassifier::_10V: %f\n", CPClassifier::_10V);
            Serial.printf("CPClassifier::_8V: %f\n", CPClassifier::_8V);
            Serial.printf("CPClassifier::_7V: %f\n", CPClassifier::_7V);
            Serial.printf("CPClassifier::_5V: %f\n", CPClassifier::_5V);
            Serial.printf("CPClassifier::_4V: %f\n", CPClassifier::_4V);
            Serial.printf("CPClassifier::_2V: %f\n", CPClassifier::_2V);
            Serial.printf("CPClassifier::_1V: %f\n", CPClassifier::_1V);
            Serial.printf("CPClassifier::_neg_1V: %f\n", CPClassifier::_neg_1V);
            Serial.printf("CPClassifier::_neg_13V: %f\n", CPClassifier::_neg_13V);
            Serial.printf("CPClassifier::_neg_11V: %f\n", CPClassifier::_neg_11V);
            break;
        default:
            break;
        }
    }

    void loop()
    {
#ifdef CALIBRATE_MODE
        if (!digitalRead(CALIBRATE_BTN_PIN))
        {
            while (!digitalRead(CALIBRATE_BTN_PIN))
            {
                Serial.print(".");
                vTaskDelay(100);
            }
            calibrate();
            counter++;
        }
#endif
        this->iter_debug = iter_pos;
        using namespace CPClassifier;
        cp_values.setCPPos(cp_pos_raw);
        cp_values.setCPNeg(cp_neg_raw);

        if (!_neg_12V(cp_values.getCPNeg()))
        {
            Serial.println("Illegal state! Negative voltage error.");
        }

        if (_9V(cp_values.getCPPos()) && _12V(cp_values.getCPPosPrev()))
        {
            Serial.println("A-->B state transition!");
            (this->A_B)();
        }

        if (_6V(cp_values.getCPPos()) && _9V(cp_values.getCPPosPrev()))
        {
            Serial.println("B-->C state transition!");
            (this->B_C)();
        }

        if (_9V(cp_values.getCPPos()) && _6V(cp_values.getCPPosPrev()))
        {
            Serial.println("C-->B state transition!");
            (this->C_B)();
        }

        if (_12V(cp_values.getCPPos()) && _9V(cp_values.getCPPosPrev()))
        {
            Serial.println("B-->A state transition!");
            (this->B_A)();
        }

        if (_3V(cp_values.getCPPos()) && _9V(cp_values.getCPPosPrev()))
        {
            Serial.println("B-->D state transition!");
            (this->B_D)();
        }

        if (_9V(cp_values.getCPPos()) && _3V(cp_values.getCPPosPrev()))
        {
            Serial.println("D-->B state transition!");
            (this->D_B)();
        }

        // TODO: error state transitions

        vTaskDelay(1);
    }
};