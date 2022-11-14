#include <Arduino.h>
#include "taskify.h"
#include "config.h"
#include "pins.h"

extern volatile uint16_t cp_max_raw;
extern volatile uint64_t cp_max_count;
void IRAM_ATTR CP_MAX_ADC_ISR();

extern volatile uint16_t cp_min_raw;
extern volatile uint64_t cp_min_count;
void IRAM_ATTR CP_MIN_ADC_ISR();

enum CP_LEVELS
{
    RANGE_12_V = 12,
    RANGE_9_V = 9,
    RANGE_6_V = 6,
    RANGE_3_V = 3,
    RANGE_0_V = 0,
    RANGE_neg_12_V = -12,
    RANGE_ILLEGAL = -12,
};

class CPVoltageReader
{
    float cp_max;
    float cp_max_prev;
    float cp_min;
    float cp_min_prev;

    void setMax(uint16_t new_cp_max_raw)
    {
        float new_cp_max = (float)((3.3f / 4096.0f) * new_cp_max_raw);
        this->cp_max_prev = this->cp_max;
        this->cp_max = new_cp_max;
    }
    void setMin(uint16_t new_cp_min_raw)
    {
        float new_cp_min = (float)((3.3f / 4096.0f) * new_cp_min_raw);
        this->cp_min_prev = this->cp_min;
        this->cp_min = new_cp_min;
    }

    inline CP_LEVELS getRange(float voltage)
    {
        if (voltage >= CP_11_V && voltage <= CP_13_V)
            return CP_LEVELS::RANGE_12_V;
        else if (voltage >= CP_8_V && voltage <= CP_10_V)
            return CP_LEVELS::RANGE_9_V;
        else if (voltage >= CP_5_V && voltage <= CP_7_V)
            return CP_LEVELS::RANGE_6_V;
        else if (voltage >= CP_2_V && voltage <= CP_4_V)
            return CP_LEVELS::RANGE_3_V;
        else if (voltage >= CP_neg_1_V && voltage <= CP_1_V)
            return CP_LEVELS::RANGE_0_V;
        else if (voltage >= CP_neg_11_V && voltage <= CP_neg_13_V)
            return CP_LEVELS::RANGE_neg_12_V;
        else
            return CP_LEVELS::RANGE_ILLEGAL;
    }

public:
    void start()
    {
        // delay(100);
        pinMode(CP_POS_ADC_TRIG_PIN, INPUT_PULLDOWN);
        pinMode(CP_NEG_ADC_TRIG_PIN, INPUT_PULLDOWN);

        // delay(100);
        attachInterrupt(CP_POS_ADC_TRIG_PIN, CP_MAX_ADC_ISR, RISING);
        attachInterrupt(CP_NEG_ADC_TRIG_PIN, CP_MIN_ADC_ISR, RISING);
        taskify("CPVoltageReader Task", 10000, this, 1, NULL);
    }
    inline float getMaxV() { return this->cp_max; }
    inline float getMinV() { return this->cp_min; }
    inline float getMaxVPrev() { return this->cp_max_prev; }
    inline float getMinVPrev() { return this->cp_min_prev; }

    inline CP_LEVELS getMaxRange() { return getRange(cp_max); }
    inline CP_LEVELS getMinRange() { return getRange(cp_min); }
    inline CP_LEVELS getMaxRangePrev() { return getRange(cp_max_prev); }
    inline CP_LEVELS getMinRangePrev() { return getRange(cp_min_prev); }

    inline float getMaxPrev() { return this->cp_max_prev; }
    inline float getMinPrev() { return this->cp_min_prev; }

    inline void loop()
    {
        setMax(cp_max_raw);
        setMin(cp_min_raw);
        vTaskDelay(10);
    }
};