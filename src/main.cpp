#include <Arduino.h>
#include "ExampleWifiClient.h"

#define PWM_PIN 16
#define PWM_PIN_2 17
#define TRIG_PIN 39
#define ADC_PIN 36
#define DAC_PIN 25
#define CONTACTOR_PIN 5

// setting PWM properties
const int freq = 100;
const int ledChannel = 0;
const int resolution = 8;

const int freq_2 = 1000;
const int ledChannel_2 = 2;
const int resolution_2 = 8;

// ADC
//#include <ESP32AnalogRead.h>
//#include <WifiServ.h>
// ESP32AnalogRead adc;

static void PWM_TASK(void *)
{
  while (1)
  {
    ledcWrite(ledChannel, 200);
    ledcWrite(ledChannel_2, 200);
    //delay(5);
    taskYIELD();
  }
}

volatile uint16_t cp_val = 0;
// volatile uint16_t cp_val_prev = 0;
uint64_t iter = 0;

void IRAM_ATTR isr()
{
  // cp_val = adc.readVoltage();
  cp_val = analogRead(ADC_PIN);
  // dacWrite(DAC_PIN, cp_val/16);
  iter++;
}

void setup()
{
  Serial.begin(115200);

  delay(100);
  // adc.attach(ADC_PIN);

  delay(100);
  pinMode(TRIG_PIN, INPUT_PULLDOWN);

  delay(100);
  attachInterrupt(TRIG_PIN, isr, RISING);

  delay(100);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(PWM_PIN, ledChannel);
  ledcAttachPin(PWM_PIN_2, ledChannel);
  //ledcSetup(ledChannel_2, freq_2, resolution_2);

  ledcWrite(ledChannel, 5);
  //ledcWrite(ledChannel_2, 200);

  //xTaskCreate(PWM_TASK, "PWM task", 1000, NULL, 1, NULL);
  // ExampleWifiClient::setup();
}

uint64_t last_read_millis = 0;
uint64_t last_read_iter = 0;

void loop()
{
  // ExampleWifiClient::loop();
  Serial.printf("Voltage MAX: %f Volts --", (float)((3.3 / 4096) * cp_val));
  Serial.printf("iter: %llu ", iter);
  Serial.printf(" laptime: %llu\n", (millis() - last_read_millis));
  last_read_iter = iter;
  last_read_millis = millis();
  delay(100);
}