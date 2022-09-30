#pragma once
#include "mtconfig.h"
#include "MTDebug.h"

#define TCP_CONTROLLER_DEBUG TRUE

#if TCP_CONTROLLER_DEBUG
#define DEBUG_LN_TCP_CONTROLLER(FORMAT, ...) \
    {                                          \
        DEBUG_LN(FORMAT, __VA_ARGS__)          \
    }
#define DEBUG_TCP_CONTROLLER(FORMAT, ...) \
    {                                       \
        DEBUG(FORMAT, __VA_ARGS__)          \
    }
#else
#define DEBUG_LN_TCP_CONTROLLER(FORMAT, ...) \
    {                                          \
    }
#define DEBUG_TCP_CONTROLLER(FORMAT, ...) \
    {                                       \
    }
#endif