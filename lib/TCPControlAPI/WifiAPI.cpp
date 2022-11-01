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
    // Connect to WiFi network
    Serial.printf("Connecting to %s", SSID);
    WiFi.begin(SSID, WIFIPASSWORD);
    delay(2000);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("%s", "WiFi connected");
    myWifiServer->begin();
    Serial.printf("%s", "Server started");
    // Print the IP address
    Serial.printf("%s", WiFi.localIP().toString().c_str());
    // Offload this->loop() calls to separate FreeRTOS task
    taskify("WIFI Task", 10000, this, 1, NULL);
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
            writeToClient(i, err_code == 0 ? "OK" : "ERROR");
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
        registerNewClient(clientCount++, newClient);
        Serial.printf("%s", "Client successfully registered!");
    }
}

void WifiAPI::registerNewClient(int clientId, WiFiClient newClient)
{
    if (NULL != clients[clientId])
    {
        Serial.printf("%s", "This connector client was already registered!");
        return;
    }
    clientCount++;
    Serial.printf("Number of clients: %d", clientCount);
    this->clients[clientId] = new WiFiClient(newClient);
}

void WifiAPI::writeToClient(int clientId, const char *message)
{
    if (this->clients[clientId] == NULL)
    {
        Serial.printf("Cannot write to client %d, client is NULL!", clientId);
        return;
    }
    this->clients[clientId]->println(message);
}

void WifiAPI::loop()
{
    checkConnectedClients();
    lookForNewClients();
    checkIncomingMessages();
    vTaskDelay(10);
}
