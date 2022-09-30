#pragma once

#include "Arduino.h"
#include <SPI.h>
#include "WiFi.h"
#include "mtconfig.h"
#include "TCPControllerDebug.h"

#define MAX_CLIENTS NUMBER_OF_CONNECTORS
#define MAX_LINE_LENGTH 50
#define MAX_ID_TIME 15000 // 15 sec

/* const int SERVER_TCP_PORT = 80; */
/* const char *SERVER_SSID = "MT_DEV";       // Enter your wifi SERVER_SSID
const char *SERVER_PWD = "MTwifi69"; // Enter your wifi SERVER_PWD */

class WifiControllerServer
{
    WiFiServer *myWifiServer;

    static WifiControllerServer *instance;

    WifiControllerServer()
    {
        this->myWifiServer = new WiFiServer(SERVER_TCP_PORT);
        setup();
    }
    ~WifiControllerServer()
    {
        delete this->myWifiServer;
    }

    void setup()
    {
        Serial.begin(115200);
        delay(10);
        // Connect to WiFi network
        DEBUG_LN_TCP_CONTROLLER("Connecting to %s", SERVER_SSID);
        /*  WiFi.on();
         WiFi.connect(); */
        WiFi.begin(SERVER_SSID, SERVER_PWD);
        delay(2000);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        DEBUG_LN_TCP_CONTROLLER("%s", "WiFi connected");
        myWifiServer->begin();
        DEBUG_LN_TCP_CONTROLLER("%s", "Server started");
        // Print the IP address
        DEBUG_LN_TCP_CONTROLLER("%s", WiFi.localIP().toString().c_str());
    }

public:
    WiFiClient *clients[MAX_CLIENTS] = {NULL};
    int clientCount = 0;

    static WifiControllerServer *getInstance()
    {
        if (!WifiControllerServer::instance)
        {
            instance = new WifiControllerServer;
        }
        return instance;
    }

    void checkConnectedClients()
    {
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            // If the client exists, but was disconnected
            if (clients[i] != NULL && !clients[i]->connected())
            {
                DEBUG_LN_TCP_CONTROLLER("Client %d was disconnected!", i);
                clients[i] = NULL;
            }
        }
    }

    int incomingByte = 0;

    void loop()
    {
        checkConnectedClients();
        lookForNewClients();
       /*  if (Serial.available() > 0)
        {
            // read the incoming byte:
            incomingByte = Serial.read();

            // say what you got:
            Serial.print("I received: ");
            Serial.println(incomingByte, DEC);
            Serial.println("writing to client 1...");
            writeToClient(1, "Hello, client 1!");
        } */
        /*  readClients(); */
    }

    void lookForNewClients()
    {
        // Check if a new client has connected
        WiFiClient newClient = myWifiServer->available();
        if (newClient)
        {
            DEBUG_LN_TCP_CONTROLLER("Found new client with ip %s.", newClient.localIP().toString().c_str());
            /*  DEBUG_LN_TCP_CONTROLLER("new client");
             IPAddress ip = newClient.localIP();
             Serial.print("IP Address: ");
             DEBUG_LN_TCP_CONTROLLER(ip); */

            int identificationStartTime = millis();
            int connectorId = 0;
            DEBUG_LN_TCP_CONTROLLER("%s", "Waiting for client to send id...");
            while (!newClient.available() /* && millis() > (identificationStartTime + MAX_ID_TIME) */)
                ;
            if (newClient.available())
            {
                char newChar = newClient.read();
                connectorId = identifyConnector(newChar);
                if (connectorId)
                {
                    DEBUG_LN_TCP_CONTROLLER("%s", "Client successfully identified!");
                    registerNewClient(connectorId, newClient);
                    newClient.println("accepted");
                }
                else
                {
                    DEBUG_LN_TCP_CONTROLLER("%s", "Client identification failed. Reason: incorrect connector id %c.", newChar);
                }
            }
            else
            {
                DEBUG_LN_TCP_CONTROLLER("%s", "Client identification failed. Reason: timeout.");
            }
        }
    }

    void registerNewClient(int connectorId, WiFiClient newClient)
    {
        if (connectorId < 1 || connectorId > NUMBER_OF_CONNECTORS - 1)
        {
            DEBUG_LN_TCP_CONTROLLER("%s", "Illegal connectorId!");
            return;
        }
        if (NULL != clients[connectorId])
        {
            DEBUG_LN_TCP_CONTROLLER("%s", "This connector client was already registered!");
            return;
        }
        clientCount++;
        Serial.printf("Number of clients: %d", clientCount);
        this->clients[connectorId] = new WiFiClient(newClient);
    }

    int identifyConnector(char c)
    {
        switch (c)
        {
        case '1':
            return 1;
        case '2':
            return 2;
        default:
            return 0;
        }
    }

    void writeToClient(int connectorId, const char *message)
    {
        if (this->clients[connectorId] == NULL)
        {
            Serial.printf("Cannot write to client %d, client is NULL!", connectorId);
            return;
        }
        if (connectorId < 1 || connectorId > MAX_CLIENTS)
        {
            Serial.printf("Illegal connector id: %d !", connectorId);
            return;
        }
        this->clients[connectorId]->println(message);
    }

    /*  void readClients()
     {
         // Check whether each client has some data
         for (int i = 0; i < MAX_CLIENTS; ++i)
         {
             // If the client is in use, and has some data...
             if (NULL != clients[i] && clients[i]->available())
             {
                 // Read the data
                 DEBUG_LN_TCP_CONTROLLER("Reading data...");
                 char newChar = clients[i]->read();
                 DEBUG_LN_TCP_CONTROLLER("Reading data 2...");
                 byteCount++;
                 if ('\r' == newChar || byteCount > MAX_LINE_LENGTH)
                 {
                     DEBUG_LN_TCP_CONTROLLER("New message: ");
                     Serial.print(inputs[i]);

                     // Empty the string for next time
                     delete inputs[i];
                     inputs[i] = new char[MAX_LINE_LENGTH]{0};
                     byteCount = 0;
                 }
                 else
                 {
                     DEBUG_LN_TCP_CONTROLLER("concating data ...");
                     // Add it to the string
                     inputs[i] = appendCharToCharArray(inputs[i], newChar);
                 }
             }
         }
     } */
};

WifiControllerServer *WIFI_CONTROLLER_SERVER();