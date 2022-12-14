#include "interrupts.h"

#include "pins.h"

volatile uint16_t cp_max_raw = 0;
volatile uint64_t cp_max_count = 0;

void IRAM_ATTR CP_MAX_ADC_ISR()
{
    cp_max_raw = analogRead(CP_POS_ADC_PIN);
    cp_max_count++;
}

volatile uint16_t cp_min_raw = 0;
volatile uint64_t cp_min_count = 0;

void IRAM_ATTR CP_MIN_ADC_ISR()
{
    cp_min_raw = analogRead(CP_NEG_ADC_PIN);
    cp_min_count++;
}

volatile uint16_t pp_raw = 0;
volatile uint64_t pp_count = 0;

void IRAM_ATTR PP_ADC_ISR()
{
    pp_raw = analogRead(PP_ADC_PIN);
    pp_count++;
}