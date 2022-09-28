#include <Arduino.h>

#define PWM_PIN 16
#define ADC_PIN 5

// setting PWM properties
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;

// ADC
#include <ESP32AnalogRead.h>
//#include <WifiServ.h>
ESP32AnalogRead adc;

static void PWM_TASK(void *)
{
  while (1)
  {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, 100);
    delay(15);
    // // increase the LED brightness
    // for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++)
    // {
    // }

    // // decrease the LED brightness
    // for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--)
    // {
    //   // changing the LED brightness with PWM
    //   ledcWrite(ledChannel, dutyCycle);
    //   delay(15);
    // }
    // delay(10);
    taskYIELD();
  }
}

uint64_t iterations = 0;
float max_adc = 0.0;

static void ADC_TASK(void *)
{
  while (1)
  {

    delay(10);
    iterations++;

    float current = adc.readVoltage();
    if (current > max_adc /* || (iterations % 5000) == 0 */)
    {
      max_adc = current;
      Serial.println("Voltage MAX = " + String(max_adc));
    }
    // delay(10);
    taskYIELD();
  }
}

void setup()
{
  Serial.begin(115200);
  delay(100);
  // mywifi::setup();
  // configure LED PWM functionalitites

  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PWM_PIN, ledChannel);

  adc.attach(33);

  xTaskCreate(PWM_TASK, "PWM task", 1000, NULL, 1, NULL);
  xTaskCreate(ADC_TASK, "ADC task", 1000, NULL, 1, NULL);
}

void loop()
{
}