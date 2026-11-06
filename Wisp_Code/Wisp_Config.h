#pragma once

/* CHANGE PARAMETERS! */
#define WISP_VERSION  2      // 1 or 2
#define WISP_INSTANCE 26     // Unit number
#define WISP_USE_LTE  false  // true or false
#define WISP_USE_WIFI true  // true or false

// MQTT server info
#define MQTT_BROKER   "143.110.159.19"
#define MQTT_PORT     1883
#define MQTT_DATABASE "test"

// Macro wizardry to turn version into string
#define STRINFIGY(x) #x
#define EXPAND_STRINGIFY(x) STRINFIGY(x)
#define WISP_NAME "Wisp_V" EXPAND_STRINGIFY(WISP_VERSION) "_"

// Number of SEN55/SEN66 samples to average per data point
#define WISP_PM_SAMPLES 10

// Minutes between samples
#define WISP_SAMPLE_MINUTES 5

// A packet is logged every 5 minutes, so mqtt will publish a batch of 72
// packets every 6 hours
#define WISP_BATCH_SIZE_MAX 72

// Sanity check configuration values
#if (                                                                          \
       defined(WISP_VERSION)                                                   \
    && defined(WISP_INSTANCE)                                                  \
    && defined(WISP_NAME)                                                      \
    && defined(WISP_USE_LTE)                                                   \
    && defined(WISP_USE_WIFI)                                                  \
    && defined(MQTT_BROKER)                                                    \
    && defined(MQTT_PORT)                                                      \
    && defined(MQTT_DATABASE)                                                  \
    && (1 <= WISP_VERSION && WISP_VERSION <= 2)                                \
    && !(WISP_USE_LTE == true && WISP_USE_WIFI == true)                        \
)
#   define WISP_CONFIG_VALID
#endif
