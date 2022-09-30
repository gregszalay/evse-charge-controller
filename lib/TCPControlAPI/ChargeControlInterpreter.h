#pragma once

#include "Arduino.h"
#include "TCPControllerDebug.h"

#define MAX_MESSAGE_LENGTH 50

class ChargeControlInterpreter
{
public:
    char *message = new char[MAX_MESSAGE_LENGTH]{0};
    Client *client = nullptr;
    int byteCount = 0;

    ChargeControlInterpreter(Client *client)
    {
        this->client = client;
    }

    ~ChargeControlInterpreter()
    {
    }

    const char *getLastMessage()
    {
        return this->message;
    }

    void *clearLastMessage()
    {
        this->message = NULL;
    }

    void loop()
    {
        // listen continuously
        if (getChargeControlMessage())
        {
            yield();
        }
    }

private:
    bool getChargeControlMessage()
    {
        if (this->client && this->client->available())
        {
            // Read the data
            DEBUG_LN_TCP_CONTROLLER("%s", "Reading data...");
            char newChar = this->client->read();
            // client.println("start charge");

            byteCount++;

            // If we have the end of a string
            // (Using the test your code uses)
            if ('\r' == newChar || byteCount > MAX_MESSAGE_LENGTH)
            {
                // Blah blah, do whatever you want with this->message
                DEBUG_LN_TCP_CONTROLLER("New message: %s", this->message);

                // Empty the string for next time
                delete this->message;
                this->message = new char[MAX_MESSAGE_LENGTH]{0};
                byteCount = 0;
                // The complete message available
                return true;
            }
            else
            {
                DEBUG_LN_TCP_CONTROLLER("%s", "concating data ...");
                // Add it to the string
                if (this->message && newChar)
                {
                    this->message = appendCharToCharArray(this->message, newChar);
                }
                else
                {
                    DEBUG_LN_TCP_CONTROLLER("%s", "this->message or newChar is NULL!");
                }
                // The complete message is not available yet
            }
            return false;
        }
    }

    char *appendCharToCharArray(char *array, char a)
    {
        size_t len = strlen(array);

        char *ret = new char[len + 2];

        strcpy(ret, array);
        ret[len] = a;
        ret[len + 1] = '\0';

        return ret;
    }
};