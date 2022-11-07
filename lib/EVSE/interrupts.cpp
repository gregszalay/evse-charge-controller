#include "interrupts.h"
#include "pins.h"

volatile uint16_t cp_pos_raw = 0;
volatile uint64_t iter_pos = 0;

void IRAM_ATTR CP_POS_ADC_ISR()
{
    cp_pos_raw = analogRead(CP_POS_ADC_PIN);
    iter_pos++;
}

volatile uint16_t cp_neg_raw = 0;
volatile uint64_t iter_neg = 0;

void IRAM_ATTR CP_NEG_ADC_ISR()
{
    cp_pos_raw = analogRead(CP_NEG_ADC_PIN);
    iter_neg++;
}