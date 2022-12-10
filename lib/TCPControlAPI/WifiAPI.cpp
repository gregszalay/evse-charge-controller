#include "WifiAPI.h"
#include "taskify.h"

WifiAPI *WifiAPI::instance = nullptr;

WifiAPI *WIFI_API()
{
    return WifiAPI::getInstance();
}

WifiAPI::WifiAPI()
{
}
WifiAPI::~WifiAPI() { delete this->myWifiServer; }

void WifiAPI::start(std::map<std::string, std::function<uint8_t()>> *tcp_message_handlers)
{
    commandHandlers = tcp_message_handlers;
    delay(10);
    Serial.printf("Connecting to %s", SSID);
    WiFi.begin(SSID, WIFIPASSWORD);
    delay(2000);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("%s\n", "WiFi connected");
    myWifiServer->begin();
    Serial.printf("%s\n", "Server started");
    Serial.printf("%s\n", WiFi.localIP().toString().c_str());
    // TODO Offload this->loop() calls to separate FreeRTOS task
    // taskify("WIFI Task", 10000, this, 1, NULL);
}

void WifiAPI::checkConnectedClients()
{
    for (int i = 0; i < MAX_CLIENTS_ALLOWED; ++i)
    {
        if (clients[i] != NULL && !clients[i]->connected())
        {
            Serial.printf("Client %d was disconnected!", i);
            clients[i] = NULL;
        }
    }
}

void WifiAPI::checkIncomingMessages()
{
    for (size_t i = 0; i < MAX_CLIENTS_ALLOWED; i++)
    {
        if (this->clients_awaiting_auth[i] != nullptr && millis() > this->clients_auth_start_time_ms[i] + MAX_AUTH_TIME)
        {
            Serial.println("Authentication timeout!");
            writeToUnauthenticatedClient(i, "Authentication timeout!");
            this->clients_awaiting_auth[i]->stop();
            delete this->clients_awaiting_auth[i];
            this->clients_awaiting_auth[i] = NULL;
            this->clientCountWaitingAuth--;
            return;
        }
        if (this->clients_awaiting_auth[i] != nullptr && this->clients_awaiting_auth[i]->available() > 0)
        {
            std::stringstream lastMessage_ss;
            while (this->clients_awaiting_auth[i] != nullptr && this->clients_awaiting_auth[i]->available() > 0)
                lastMessage_ss << (char)this->clients_awaiting_auth[i]->read();
            std::string incoming_message_str = lastMessage_ss.str().substr(0, lastMessage_ss.str().size() - 1); // remove the "\n" character
            int incoming_message_size = incoming_message_str.size();
            const char *incoming_message = incoming_message_str.c_str();
            Serial.printf("Received PASSWORD message from client %d:\n%s\n", i, incoming_message); // PASSWORD print only for testing purposes
            if (incoming_message_size != SERVERPASSWORDSIZE)
            {
                Serial.println("Authentication failed! Wrong password size!"); // Only for testing
                Serial.printf("Actual size of incoming message: %d", incoming_message_size);
                writeToUnauthenticatedClient(i, "Authentication failed!");
            }
            else if (strncmp(SERVERPASSWORD, incoming_message, SERVERPASSWORDSIZE) != 0)
            {
                Serial.println("Authentication failed!");
                writeToUnauthenticatedClient(i, "Authentication failed! Wrong password!");
            }
            else
            {
                Serial.println("Authentication successful!");
                registerAuthenticatedClient(i, this->clients_awaiting_auth[i]);
                writeToAuthenticatedClient(i, "Authentication successful!");
            }
        }

        if (this->clients[i] != nullptr && this->clients[i]->available() > 0)
        {
            std::stringstream lastMessage_ss;
            while (this->clients[i] != nullptr && this->clients[i]->available() > 0)
                lastMessage_ss << (char)this->clients[i]->read();
            std::string incoming_message = lastMessage_ss.str();
            Serial.printf("Received message from client %d:\n%s\n", i, incoming_message.c_str());
            if (commandHandlers->find(incoming_message) == commandHandlers->end())
            {
                Serial.printf("Response not implemented for message %s!\n", incoming_message.c_str());
                continue;
            }
            uint8_t err_code = (*commandHandlers)[incoming_message]();
            // TODO error handling
        }
    }
}

void WifiAPI::lookForNewClients()
{
    // Check if a new client has connected
    WiFiClient newClient = myWifiServer->available();
    if (newClient)
    {
        Serial.printf("Found new client with ip %s.", newClient.localIP().toString().c_str());
        this->clientCountWaitingAuth++;
        registerNewClientForAuth(this->clientCountWaitingAuth - 1, newClient);
        Serial.printf("%s", "Waiting for client authentication...\n");
    }
}

void WifiAPI::registerNewClientForAuth(int clientId, WiFiClient newClient)
{
    if (NULL != clients_awaiting_auth[clientId] || NULL != clients[clientId])
    {
        Serial.printf("%s", "This connector client was already registered!\n");
        return;
    }
    Serial.printf("Number of clients waiting for authentication: %d\n", clientCountWaitingAuth);
    this->clients_awaiting_auth[clientId] = new WiFiClient(newClient);
    this->clients_auth_start_time_ms[clientId] = millis();
}

void WifiAPI::registerAuthenticatedClient(int clientId, WiFiClient *newClient)
{
    if (NULL != clients[clientId])
    {
        Serial.printf("%s", "This connector client was already registered!\n");
        return;
    }
    Serial.printf("Number of clients: %d", ++this->clientCount);
    this->clients[clientId] = newClient;
    // delete this->clients_awaiting_auth[clientId];
    this->clients_awaiting_auth[clientId] = NULL;
}

void WifiAPI::writeToAuthenticatedClient(int clientId, const char *message)
{
    if (this->clients[clientId] == NULL)
    {
        Serial.printf("Cannot write to client %d, client is NULL!\n", clientId);
        return;
    }
    this->clients[clientId]->println(message);
}

void WifiAPI::writeToUnauthenticatedClient(int clientId, const char *message)
{
    if (this->clients_awaiting_auth[clientId] == NULL)
    {
        Serial.printf("Cannot write to unauthenticated client %d, client is NULL!\n", clientId);
        return;
    }
    this->clients_awaiting_auth[clientId]->println(message);
}

void WifiAPI::loop()
{
    checkConnectedClients();
    lookForNewClients();
    checkIncomingMessages();
    vTaskDelay(10);
    delay(50);
}
