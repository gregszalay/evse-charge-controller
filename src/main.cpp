#include <Arduino.h>
#include "WifiAPI.h"
#include "pins.h"
#include "EVSE.h"
#include <functional>
#include <map>

EVSE evse;

// WiFi commands:
const char *start_command = "start\n";
const char *stop_command = "stop\n";
const char *status_req = "status?\n";

std::map<std::string, std::function<uint8_t()>> *tcp_message_handlers =
    new std::map<std::string, std::function<uint8_t()>>{
        /******************************************************************/
        {start_command,
         []() -> uint8_t
         {
           Serial.print("Starting charge");
           evse.setChargingAllowed(true);
           return 0;
         }},
        /******************************************************************/
        {stop_command,
         []() -> uint8_t
         {
           Serial.print("Stopping charge");
           evse.setChargingAllowed(false);
           return 0;
         }},
        {status_req,
         []() -> uint8_t
         {
           Serial.print("Sending status");
           WIFI_API()->writeToClient(0, evse.current_status.toString().c_str());
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
  evse.start();
}

void loop()
{
  delay(100);
}