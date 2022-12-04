#include "Arduino.h"

extern volatile uint16_t cp_max_raw;
extern volatile uint64_t cp_max_count;
void IRAM_ATTR CP_MAX_ADC_ISR();

extern volatile uint16_t cp_min_raw;
extern volatile uint64_t cp_min_count;
void IRAM_ATTR CP_MIN_ADC_ISR();

extern volatile uint16_t pp_raw;
extern volatile uint64_t pp_count;
void IRAM_ATTR PP_ADC_ISR();