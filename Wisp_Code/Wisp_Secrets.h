#pragma once

#include "Wisp_Config.h"
#ifndef WISP_CONFIG_VALID
#   error Invalid configuration in "Wisp_Config.h"
#endif

// WiFi netowrk credentials
#if WISP_USE_WIFI
#   define WIFI_SSID     ""
#   define WIFI_PASSWORD ""
#endif

// MongoDB Credentials
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""

// Ensure valid configuration
#if (                                                                          \
       defined(MQTT_USERNAME)                                                  \
    && defined(MQTT_PASSWORD)                                                  \
    && (                                                                       \
           WISP_USE_WIFI                                                       \
        && defined(WIFI_SSID)                                                  \
        && defined(WIFI_PASSWORD)                                              \
    )                                                                          \
)
#   define WISP_SECRETS_VALID
#endif
