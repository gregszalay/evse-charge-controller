#include "WifiControllerServer.h"

WifiControllerServer *WifiControllerServer::instance = nullptr;

WifiControllerServer *WIFI_CONTROLLER_SERVER()
{
    return WifiControllerServer::getInstance();
}