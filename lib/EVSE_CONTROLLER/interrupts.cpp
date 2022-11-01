#include "interrupts.h"

volatile uint16_t cp_pos_raw = 0;
volatile uint16_t cp_neg_raw = 0;
volatile uint64_t iter = 0;

void IRAM_ATTR CP_POS_ADC_ISR()
{
    cp_pos_raw = analogRead(36);
    iter++;
}