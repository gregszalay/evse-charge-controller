#pragma once

#include "Arduino.h"
//#include <SPI.h>
#include <string>
#include <map>
#include <functional>
#include <sstream> // std::stringstream
#include "WiFi.h"
#include "PRIVATE.h"

#define MAX_CLIENTS_ALLOWED 1
#define MAX_LINE_LENGTH 50
#define MAX_ID_TIME 15000 // 15 sec
const int SERVER_TCP_PORT = 80;

class WifiAPI
{
    WiFiServer *myWifiServer = new WiFiServer(SERVER_TCP_PORT);
    static WifiAPI *instance;
    WifiAPI();
    ~WifiAPI();
    WiFiClient *clients[MAX_CLIENTS_ALLOWED] = {NULL};
    int clientCount = 0;
    std::map<std::string, std::function<uint8_t()>> *commandHandlers = nullptr;

public:
    static WifiAPI *getInstance()
    {
        if (!WifiAPI::instance)
        {
            instance = new WifiAPI;
        }
        return instance;
    }
    void start(std::map<std::string, std::function<uint8_t()>> *tcp_message_handlers);
    void checkConnectedClients();
    void checkIncomingMessages();
    void loop();
    void lookForNewClients();
    void registerNewClient(int clientId, WiFiClient newClient);
    void writeToClient(int clientId, const char *message);
};

WifiAPI *WIFI_API();