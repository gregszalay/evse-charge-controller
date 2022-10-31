#include <Arduino.h>
#include "ExampleWifiClient.h"
#include "ChargeController.h"

ChargeController evse_controller(1);

void setup()
{
  if (!Serial)
    Serial.begin(115200);

  delay(100);
  // ExampleWifiClient::setup();
  delay(100);
}

void loop()
{
  // ExampleWifiClient::loop();
  float cp_volts = evse_controller.cp.cp_values.getCPPos();
  uint64_t iterations = evse_controller.cp.iter;
  Serial.printf("Voltage MAX: %f Volts --", cp_volts);
  Serial.printf("iter: %llu ", iterations);
  delay(100);
}