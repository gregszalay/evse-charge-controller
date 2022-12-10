#include <Arduino.h>
#include "WifiAPI.h"
#include "pins.h"
#include "EVSE.h"
#include "taskify.h"
#include <functional>
#include <map>
#include <string>

EVSE evse;
// This implementation currently supports charge control by only 1 client.
// In the future, control by multiple clients could be possible.
#define EVSE_WIFI_CONTROL_CLIENT 0

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
           String result("");
           result += String("command") + String(":");
           result += String("OK") + String(",");
           WIFI_API()->writeToAuthenticatedClient(EVSE_WIFI_CONTROL_CLIENT, result.c_str());
           // TODO: implement error response
           return 0;
         }},
        /******************************************************************/
        {stop_command,
         []() -> uint8_t
         {
           Serial.print("Stopping charge");
           evse.setChargingAllowed(false);
           String result("");
           result += String("command") + String(":");
           result += String("OK") + String(",");
           WIFI_API()->writeToAuthenticatedClient(EVSE_WIFI_CONTROL_CLIENT, result.c_str());
           // TODO: implement error response
           return 0;
         }},
        {status_req,
         []() -> uint8_t
         {
           Serial.print("Sending status");
           String result("");
           result += String("status") + String(":");
           result += String(evse.status.isEVConnected) + String(",");
           result += String(evse.status.isChargingEnabled) + String(",");
           result += String(evse.status.isCharging) + String(",");
           result += String(evse.status.error) + String(",");
           WIFI_API()->writeToAuthenticatedClient(EVSE_WIFI_CONTROL_CLIENT, result.c_str());
           return 0;
         }},
        {metervalues_req,
         []() -> uint8_t
         {
           Serial.print("Sending metervalues");
           String result("");
           result += String("metervalues") + String(":");
           result += String(evse.status.energy) + String(","); // Energy active net
           result += String(evse.status.power) + String(",");  // Power active import
           WIFI_API()->writeToAuthenticatedClient(EVSE_WIFI_CONTROL_CLIENT, result.c_str());
           return 0;
         }},
    };

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting charge controller...");
  delay(1000);
  WIFI_API()->start(tcp_message_handlers);
  delay(2000);
  evse.start();
  delay(2000);
}

void loop()
{
  WIFI_API()->loop();
  delay(10);
}