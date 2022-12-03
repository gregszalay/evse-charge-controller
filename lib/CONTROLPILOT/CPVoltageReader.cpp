#include "CPVoltageReader.h"
#include "pins.h"

volatile uint16_t cp_max_raw = 0;
volatile uint64_t cp_max_count = 0;

void IRAM_ATTR CP_MAX_ADC_ISR()
{
    // if (xSemaphoreTakeFromISR(xSemaphore, NULL) == pdTRUE)
    // {
    cp_max_raw = analogRead(CP_POS_ADC_PIN);
    cp_max_count++;
   // xSemaphoreGiveFromISR(xSemaphore, NULL);
    // }
    // else
    // {
    // }
}

volatile uint16_t cp_min_raw = 0;
volatile uint64_t cp_min_count = 0;

void IRAM_ATTR CP_MIN_ADC_ISR()
{
    // if (xSemaphoreTakeFromISR(xSemaphore, NULL) == pdTRUE)
    // {
    cp_min_raw = analogRead(CP_NEG_ADC_PIN);
    cp_min_count++;
   // xSemaphoreGiveFromISR(xSemaphore, NULL);
    // }
    // else
    // {
    // }
}