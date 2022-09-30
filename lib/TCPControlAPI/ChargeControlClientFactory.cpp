#include "ChargeControlClientFactory.h"

ChargeControlClientFactory *ChargeControlClientFactory::instance = nullptr;

ChargeControlClientFactory *CLIENT_FACTORY()
{
    return ChargeControlClientFactory::getInstance();
}