#include <Arduino.h>
#include "ExampleWifiClient.h"
#include "ChargeController.h"

ChargeController evse_controller(1);

void setup()
{
  Serial.begin(115200);

  Serial.println("Starting charge controller...");

  delay(100);
  // ExampleWifiClient::setup();
  delay(100);
  evse_controller.cp.setup();
}

void loop()
{
  // ExampleWifiClient::loop();
  float cp_volts = evse_controller.cp.cp_values.getCPPos();
  uint64_t iterations = evse_controller.cp.iter_debug;
  Serial.printf("Voltage MAX: %f Volts --", cp_volts);
  Serial.printf("iter: %llu \n", iterations);
  delay(100);
}