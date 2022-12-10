#pragma once
#include "Arduino.h"
#include <string>
#include <map>
#include <functional>
#include <sstream> // std::stringstream
#include <map>
#include "WiFi.h"
#include "PRIVATE.h"

#define MAX_CLIENTS_ALLOWED 1 // The maximum number of clients allowed to connect at any time
#define MAX_AUTH_TIME 5000    // The client has this long to authenticate itself before the server closes the connection
#define MAX_LINE_LENGTH 50
const int SERVER_TCP_PORT = 80;

class WifiAPI
{
    WiFiServer *myWifiServer = new WiFiServer(SERVER_TCP_PORT);
    static WifiAPI *instance;
    WifiAPI();
    ~WifiAPI();
    WiFiClient *clients[MAX_CLIENTS_ALLOWED] = {NULL};
    WiFiClient *clients_awaiting_auth[MAX_CLIENTS_ALLOWED] = {NULL};
    int clients_auth_start_time_ms[MAX_CLIENTS_ALLOWED] = {NULL};
    int clientCount = 0;
    int clientCountWaitingAuth = 0;
    std::map<std::string, std::function<uint8_t()>> *commandHandlers = nullptr;

    void checkConnectedClients();
    void checkIncomingMessages();
    void lookForNewClients();
    void registerAuthenticatedClient(int clientId, WiFiClient *newClient);
    void registerNewClientForAuth(int clientId, WiFiClient newClient);
    void writeToUnauthenticatedClient(int clientId, const char *message);

public:
    static WifiAPI *getInstance()
    {
        if (!WifiAPI::instance)
            instance = new WifiAPI;
        return instance;
    }
    void start(std::map<std::string, std::function<uint8_t()>> *tcp_message_handlers);
    void loop();
    void writeToAuthenticatedClient(int clientId, const char *message);
};

WifiAPI *WIFI_API();