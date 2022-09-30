#include "WiFi.h"

const char *ssid = "MT_DEV";
const char *password = "MTwifi69";

namespace ExampleWifiClient
{

    WiFiClient client;

    void setup()
    {

        Serial.begin(115200);

        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.println("Connecting to WiFi..");
        }

        Serial.println("Connected to the WiFi network");

        if (client.connect("www.google.com", 80))
        {

            Serial.println("connected to server");

            // Make a HTTP request:

            client.println("GET / HTTP/1.1");

            client.println("Host: www.gergelyszalay.hu");

            client.println("Connection: close");

            client.println();
        }
    }

    void loop()
    {

        // if there are incoming bytes available

        // from the server, read them and print them:

        while (client.available())
        {

            char c = client.read();

            Serial.write(c);
        }

        // if the server's disconnected, stop the client:

        if (!client.connected())
        {

            Serial.println();

            Serial.println("disconnecting from server.");

            client.stop();

            // do nothing forevermore:

            while (true)
                ;
        }
    }
}