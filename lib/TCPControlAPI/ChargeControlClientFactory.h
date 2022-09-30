#pragma once

#include "Arduino.h"
#include "WiFi.h"
#include "mtconfig.h"

#include "TCPControllerDebug.h"

enum StreamTypes
{
    WIFI_CLIENT,
    HARDWARE_SERIAL,
};

#define USE_SERIAL Serial1

class ChargeControlClientFactory
{

    static ChargeControlClientFactory *instance;

    ChargeControlClientFactory()
    {
    }
    ~ChargeControlClientFactory()
    {
    }

public:
    static ChargeControlClientFactory *getInstance()
    {
        if (!ChargeControlClientFactory::instance)
        {
            instance = new ChargeControlClientFactory;
        }
        return instance;
    }

    Client *produceAndConnectClient(StreamTypes type)
    {
        switch (type)
        {
        case StreamTypes::WIFI_CLIENT:
            return buildWiFiClient();
            break;
            /*  case StreamTypes::HARDWARE_SERIAL:
                 return buildHardwareSerialClient();
                 break; */
        default:
            return buildWiFiClient();
        }
    }

private:
    WiFiClient *buildWiFiClient()
    {
        WiFiClient *client = new WiFiClient;
        int status = WL_IDLE_STATUS;
        while (status != WL_CONNECTED)
        {
            DEBUG_LN_TCP_CONTROLLER("Attempting to connect to SERVER_SSID: %s", SERVER_SSID);
            // Connect to WPA/WPA2 network.
            status = WiFi.begin(SERVER_SSID, SERVER_PWD);
            // wait 10 seconds for connection:
            delay(5000);
        }
        DEBUG_LN_TCP_CONTROLLER("%s", "Connected to wifi");
        DEBUG_LN_TCP_CONTROLLER("%s", "Starting connection to server...");
        if (client->connect(SERVER_IP, SERVER_TCP_PORT))
        {
            DEBUG_LN_TCP_CONTROLLER("%s", "New WiFi client connected to server!");
        }
        return client;
    }

    /* HardwareSerial *buildHardwareSerialClient()
    {
        USE_SERIAL.flush();
        USE_SERIAL.begin(115200);

        while (!USE_SERIAL)
        {

            ; // wait for serial SERVER_TCP_PORT to connect. Needed for native USB SERVER_TCP_PORT only
        }
        return &USE_SERIAL;
    } */
};

ChargeControlClientFactory *CLIENT_FACTORY();