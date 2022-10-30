#include <Arduino.h>
// #include "ExampleWifiClient.h"

#define PWM_PIN 16
#define PWM_PIN_2 17
#define TRIG_PIN 39
#define ADC_PIN 36
#define DAC_PIN 25

// setting PWM properties
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;

const int freq_2 = 1000;
const int ledChannel_2 = 2;
const int resolution_2 = 8;

// ADC
#include <ESP32AnalogRead.h>
//#include <WifiServ.h>
ESP32AnalogRead adc;

static void PWM_TASK(void *)
{
  while (1)
  {
    ledcWrite(ledChannel, 200);
    ledcWrite(ledChannel_2, 200);
    delay(5);
    taskYIELD();
  }
}

volatile uint16_t cp_val = 0;
uint64_t iter = 0;

void IRAM_ATTR isr()
{
  // cp_val = adc.readVoltage();
  cp_val = analogRead(ADC_PIN);
  dacWrite(DAC_PIN, cp_val/16);
  iter++;
}

void setup()
{
  Serial.begin(115200);

  delay(100);
  adc.attach(ADC_PIN);

  delay(100);
  pinMode(TRIG_PIN, INPUT_PULLDOWN);

  delay(100);
  attachInterrupt(TRIG_PIN, isr, RISING);

  delay(100);
  ledcAttachPin(PWM_PIN, ledChannel);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(PWM_PIN_2, ledChannel_2);
  ledcSetup(ledChannel_2, freq_2, resolution_2);

  xTaskCreate(PWM_TASK, "PWM task", 1000, NULL, 1, NULL);
}

void loop()
{
  Serial.printf("Voltage MAX: %f Volts --", (float)((3.3 / 4096) * cp_val));
  Serial.printf("iter: %llu\n", iter);
  delay(10);
}