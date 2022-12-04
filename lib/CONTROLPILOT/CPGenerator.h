#include <Arduino.h>
#include "pins.h"

class CPGenerator
{
    uint32_t FREQ_HZ = 1000;
    uint8_t DEFAULT_DUTY_CYCLE_PERCENT = 80;
    uint8_t RESOLUTION = 8;
    uint8_t CHANNEL = 0;
    uint8_t current_duty_cycle_percent = 255;

public:
    void start()
    {
        ledcSetup(CHANNEL, FREQ_HZ, RESOLUTION);
        ledcAttachPin(PWM_PIN, CHANNEL);
        ledcAttachPin(PWM_PIN_2, CHANNEL);
        ledcAttachPin(PWM_PIN_3, CHANNEL);
        setDutyCyclePercent(DEFAULT_DUTY_CYCLE_PERCENT);
    }

    void setDutyCyclePercent(uint8_t percent)
    {
        // No change
        if (percent == current_duty_cycle_percent)
            return;
        // Illegal argument
        if (percent > 100)
        {
            Serial.println("Error: Illegal for duty cycle. Max duty cycle is 100%");
            return;
        }
        uint32_t new_value = uint32_t(255.0f / 100.0f * percent);
        Serial.printf("Setting duty cycle to %u percent (%u resolution)\n", percent, new_value);
        ledcWrite(CHANNEL, new_value);
        this->current_duty_cycle_percent = percent;
    };

    uint8_t getDutyCyclePercent() { return this->current_duty_cycle_percent; }
};