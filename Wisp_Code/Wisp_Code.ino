#include "Wisp_Config.h"
#ifndef WISP_CONFIG_VALID
#   error Invalid configuration in "Wisp_Config.h"
#endif

#include <Loom_Manager.h>
#include <Adafruit_SleepyDog.h>
#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>
#include <Logger.h>
#include <Sensors/Loom_Analog/Loom_Analog.h>

#define WISP_USE_NETWORK (WISP_USE_LTE == true || WISP_USE_WIFI == true)

#if WISP_USE_NETWORK
#   include <Internet/Logging/Loom_MongoDB/Loom_MongoDB.h>
#   include "Wisp_Secrets.h"
#   ifndef WISP_SECRETS_VALID
#       error Invalid configuration in "Wisp_Secrets.h"
#   endif
#endif

#if WISP_USE_LTE
#   include <Internet/Connectivity/Loom_LTE/Loom_LTE.h>
#elif WISP_USE_WIFI
#   include <Internet/Connectivity/Loom_Wifi/Loom_Wifi.h>
#endif

#if WISP_VERSION==1
#   include <Sensors/I2C/Loom_SEN55/Loom_SEN55.h>
#   include <Sensors/I2C/Loom_SHT31/Loom_SHT31.h>
#elif WISP_VERSION==2
#   include <Hardware/Loom_Multiplexer/Loom_Multiplexer.h>
#   define I2C_ADDR_DFGAS 0x74
#   define I2C_ADDR_SEN66 0x6B
#   define I2C_ADDR_SHT31 0x44
#endif

Manager manager(WISP_NAME, WISP_INSTANCE);
Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST, true);

// Reads the battery voltage
Loom_Analog analog(manager);

#if WISP_USE_LTE
Loom_LTE network(manager, "hologram", "", "");
#endif

#if WISP_USE_WIFI
Loom_WIFI network(manager, CommunicationMode::CLIENT, WIFI_SSID, WIFI_PASSWORD);
#endif

#if WISP_USE_NETWORK
Loom_MongoDB mqtt(
    manager, network,
    MQTT_BROKER, MQTT_PORT, MQTT_DATABASE, MQTT_USERNAME, MQTT_PASSWORD
);
uint8_t currentBatchSize = 2;
uint8_t targetBatchSize = 2;
#endif

#if WISP_VERSION==1
// Main air quality, temperature, and humidity sensing
Loom_SEN55 SEN55(manager);
Loom_SHT31 sht(manager);

#elif WISP_VERSION==2
/* Initialize I2C sensors through multiplexer */
Loom_Multiplexer mux(manager, {I2C_ADDR_DFGAS, I2C_ADDR_SEN66, I2C_ADDR_SHT31});
#endif

void isrTrigger()
{
  hypnos.wakeup();
}

void setup() {

  ENABLE_SD_LOGGING;

  /* DISABLE FUNCTION SUMMARIES FOR FIELD DEPLOYMENT!
   * Function summaries are disabled to prevent excessive writing to SD card
   * as well as possible memory leak during deployment.
   * This issue may be fixed after merge with main, test later. */
  // ENABLE_FUNC_SUMMARIES;

  // Start the serial interface
  manager.beginSerial();

  // Both power rails should be on when awake
  hypnos.setWakeConfiguration(POWERRAIL_CONFIG::PR_3V_ON_5V_ON);

  /* Both rails should be on during sleep to minimize effects of power surges
   * when turning on rails during wakeup. */
  hypnos.setSleepConfiguration(POWERRAIL_CONFIG::PR_3V_ON_5V_ON);

  // Enable the hypnos rails
  hypnos.enable();

  // Initialize all modules
  // LTE initialization takes ~15 seconds, do this BEFORE starting the Watchdog
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);

#if WISP_USE_NETWORK
  // Time sync using network interface
  hypnos.setNetworkInterface(&network);
  hypnos.networkTimeUpdate();

  // Set the LTE board to only powerup when a batch is ready to be sent
  network.setBatchSD(batchSD);
#endif
}

void loop() {
#if WISP_VERSION==2
  // Enable watchdog to prevent hang in measurement or logging
  Watchdog.enable(16000);
  Watchdog.reset();
#endif

  // Measure the data from the sensors
  manager.measure();

#if WISP_VERSION==1
  // Enable watchdog to prevent hang in logging, but SEN55 will time out
  Watchdog.enable(16000);
#endif

  // Pet the dog again just in case measure took a few seconds
  Watchdog.reset();

  // Package the data into JSON
  manager.package();

  // Print the JSON document to the Serial monitor
  manager.display_data();

  // Log the data to the SD
  hypnos.logToSD();

  // Disable watchdog before transmitting 4G data, this can take some time
  Watchdog.disable();

#if WISP_USE_NETWORK
  /* After sd.log() trnucates log file, apply target batch size */
  if (batchSD.getCurrentBatch() == 1 && targetBatchSize != currentBatchSize) {
    currentBatchSize = targetBatchSize;
    batchSD = Loom_BatchSD(hypnos, currentBatchSize);
  }

  // Pass in the batchSD to the mqtt obj to check/ publish a batch of data if ready
  mqtt.publish(batchSD);

  /* Ramp up target batch size after each transmission, but don't apply it yet */
  if (batchSD.shouldPublish()) {
    targetBatchSize = min(WISP_BATCH_SIZE_MAX, 2 * currentBatchSize);
  }

  // Sync time (network updates can also block for several seconds)
  hypnos.networkTimeUpdate();
#endif

  // Set the interrupt duration
  hypnos.setInterruptDuration(TimeSpan(0, 0, WISP_SAMPLE_MINUTES, 0));

  // Reattach the interrupt
  hypnos.reattachRTCInterrupt();

  // Set the hypnos to sleep, don't wait for user to open serial monitor
  hypnos.sleep(false);
}
