#include <Arduino.h>
#include "pins.h"

class CPGenerator
{
    uint32_t FREQ_HZ = 1000;
    uint8_t DEFAULT_DUTY_CYCLE_PERCENT = 50;
    uint8_t RESOLUTION = 8;
    uint8_t CHANNEL = 0;

public:
    void start()
    {
        ledcSetup(CHANNEL, FREQ_HZ, RESOLUTION);
        ledcAttachPin(PWM_PIN, CHANNEL);
        ledcAttachPin(PWM_PIN_2, CHANNEL);
        setDutyCycle(DEFAULT_DUTY_CYCLE_PERCENT);
    }

    void setDutyCycle(uint8_t percent)
    {
        if (percent > 100)
        {
            Serial.println("Error: Illegal value for duty cycle. Max duty cycle is 100%");
            return;
        }
        uint32_t new_value = uint32_t(255.0f / 100.0f * percent);
        Serial.printf("Setting duty cycle to %u percent (%u resolution)\n", percent, new_value);
        ledcWrite(CHANNEL, new_value);
    };
};