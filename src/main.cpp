#include <Arduino.h>
#include "ExampleWifiClient.h"

#define PWM_OUT_PIN 16
#define PWM_OUT_PIN_2 17
#define TRIG_PIN 39
#define ADC_PIN 36
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

volatile uint16_t cp_val = 0;
// volatile uint16_t cp_val_prev = 0;
uint64_t iter = 0;

static void WIFI_TASK(void *)
{
  while (1)
  {
    if ((float)((3.3 / 4096) * cp_val) >= 3.0)
    {
      // Serial.println("true");
      // Serial.println(cp_volts);
      digitalWrite(CONTACTOR_PIN, HIGH);
    }
    else
    {
      // Serial.println("false");
      // Serial.println(cp_volts);
      digitalWrite(CONTACTOR_PIN, LOW);
    }
    delay(7);
    taskYIELD();
  }
}

void IRAM_ATTR CP_ADC_ISR()
{
  // cp_val = adc.readVoltage();
  cp_val = analogRead(ADC_PIN);
  delayMicroseconds(2);

  iter++;
}

void setup()
{
  Serial.begin(115200);

  delay(100);
  // ExampleWifiClient::setup();
  delay(100);

  delay(100);
  pinMode(TRIG_PIN, INPUT_PULLDOWN);

  delay(100);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(PWM_OUT_PIN, ledChannel);
  ledcAttachPin(PWM_OUT_PIN_2, ledChannel);
  ledcWrite(ledChannel, 5);

  pinMode(CONTACTOR_PIN, OUTPUT);
  digitalWrite(CONTACTOR_PIN, HIGH);

  delay(100);
  attachInterrupt(TRIG_PIN, CP_ADC_ISR, RISING);
  xTaskCreate(WIFI_TASK, "WIFI task", 1000, NULL, 1, NULL);
}

uint64_t last_read_millis = 0;
uint64_t last_read_iter = 0;

void loop()
{
  // ExampleWifiClient::loop();
  float cp_volts = (float)((3.3 / 4096) * cp_val);
  Serial.printf("Voltage MAX: %f Volts --", cp_volts);
  Serial.printf("iter: %llu ", iter);
  Serial.printf(" laptime: %llu\n", (millis() - last_read_millis));
  last_read_iter = iter;
  last_read_millis = millis();

  delay(100);
}