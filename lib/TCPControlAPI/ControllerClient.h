#pragma once

#include "Arduino.h"
#include "ChargeControlClientFactory.h"
#include "ChargeControlInterpreter.h"
#include "Type2Controller.h"
#include "TCPControllerDebug.h"

class ControllerClient
{
public:
    int connectorId;
    Client *commandClient = nullptr;
    ChargeControlInterpreter *commandInterpreter = nullptr;
    bool acceptedByServer = false;
    const char *lastMessage = nullptr;

    ControllerClient(int connectorId)
    {
        this->connectorId = connectorId;
        this->commandClient = CLIENT_FACTORY()->produceAndConnectClient(StreamTypes::WIFI_CLIENT);
        this->commandInterpreter = new ChargeControlInterpreter(this->commandClient);
        // Send connector id so the server can identify which connector this is
        connectToControllerServer();
    }

    ~ControllerClient()
    {
    }

    void connectToControllerServer()
    {
        // Infinite loop
        DEBUG_LN_TCP_CONTROLLER("Connector %d is waiting to identify itself to the server...", this->connectorId);
        while (!this->acceptedByServer)
        {
            // Send id
            this->commandClient->write(this->connectorId + 48);
            delay(100);
            // Check for new message from server
            this->commandInterpreter->loop();
            // Check if server accepted
            if (strcmp(this->commandInterpreter->getLastMessage(), "accepted"))
            {
                DEBUG_LN_TCP_CONTROLLER("Connector %d was accepted by the server!", this->connectorId);
                this->acceptedByServer = true;
                // Clear last message
                this->commandInterpreter->clearLastMessage();
                this->lastMessage = nullptr;
                return;
            }
            delay(500);
        }
    }

    void loop()
    {
        // Are we connected and identified?
        if (!this->commandClient->connected() || !this->acceptedByServer)
        {
            DEBUG_LN_TCP_CONTROLLER("Client %d was disconnected!", this->connectorId);
            this->acceptedByServer = false;
            // Try to reconnect
            connectToControllerServer();
            return;
        }
        this->lastMessage = this->commandInterpreter->getLastMessage();
        // Check for new message from server
        this->commandInterpreter->loop();
        // Did we get a new message?
        if (lastMessage && this->commandInterpreter->getLastMessage() &&
            !strcmp(lastMessage, this->commandInterpreter->getLastMessage()))
        {
            DEBUG_LN_TCP_CONTROLLER("New message received by client %d.", this->connectorId);
            DEBUG_LN_TCP_CONTROLLER("The message: %s", this->commandInterpreter->getLastMessage());
            takeAction(this->commandInterpreter->getLastMessage());
        }
        delay(500);
    }

    virtual void takeAction(const char *command){};
};