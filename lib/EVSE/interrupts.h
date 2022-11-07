#include <Arduino.h>

extern volatile uint16_t cp_pos_raw;
extern volatile uint64_t iter_pos;
void IRAM_ATTR CP_POS_ADC_ISR();

extern volatile uint16_t cp_neg_raw;
extern volatile uint64_t iter_neg;
void IRAM_ATTR CP_NEG_ADC_ISR();