#include <Arduino.h>
#include "WifiAPI.h"
#include "pins.h"
#include "EVSE.h"
#include "taskify.h"
#include <functional>
#include <map>
#include <string>

EVSE evse;

// WiFi commands:
const char *start_command = "start\n";
const char *stop_command = "stop\n";
const char *status_req = "status?\n";
const char *metervalues_req = "metervalues?\n";

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
           String result("");
           result += String("status") + String(":");
           result += String(evse.current_status.isEVConnected) + String(",");
           result += String(evse.current_status.isChargingEnabled) + String(",");
           result += String(evse.current_status.isCharging) + String(",");
           result += String(evse.current_status.error) + String(",");
           WIFI_API()->writeToClient(0, result.c_str());
           return 0;
         }},
        {metervalues_req,
         []() -> uint8_t
         {
           Serial.print("Sending metervalues");
           String result("");
           result += String("metervalues") + String(":");
           result += String(1425) + String(",");
           result += String(2389) + String(",");
           WIFI_API()->writeToClient(0, result.c_str());
           return 0;
         }},
    };

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting charge controller...");
  xSemaphore = xSemaphoreCreateBinary();
  delay(1000);
  // WIFI_API()->start(tcp_message_handlers);
  delay(2000);
  evse.start();
  delay(2000);
}

void loop()
{
  // WIFI_API()->loop();
  delay(10);
}