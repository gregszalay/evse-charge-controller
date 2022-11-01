#include <Arduino.h>

extern volatile uint16_t cp_pos_raw;
extern volatile uint16_t cp_neg_raw;
extern volatile uint64_t iter;

void IRAM_ATTR CP_POS_ADC_ISR();