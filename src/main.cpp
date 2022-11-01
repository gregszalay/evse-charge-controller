#include <Arduino.h>
#include "WifiAPI.h"
#include "EVSE.h"
#include <functional>
#include <map>

EVSE evse(1);

std::map<std::string, std::function<uint8_t()>> *tcp_message_handlers =
    new std::map<std::string, std::function<uint8_t()>>{
        /******************************************************************/
        {"start\n",
         []() -> uint8_t
         {
           Serial.print("Starting charge");
           evse.setChargingAllowed(true);
           return 0;
         }},
        /******************************************************************/
        {"stop\n",
         []() -> uint8_t
         {
           Serial.print("Stopping charge");
           evse.setChargingAllowed(false);
           return 0;
         }},
    };

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting charge controller...");
  delay(100);
  WIFI_API()->start(tcp_message_handlers);
  delay(100);
  evse.cp.setup();
}

void loop()
{
  float cp_volts = evse.cp.cp_values.getCPPos();
  uint64_t iterations = evse.cp.iter_debug;
  Serial.printf("Voltage MAX: %f Volts --", cp_volts);
  Serial.printf("iter: %llu \n", iterations);
  delay(100);
}