#include <Loom_Manager.h>
#include <Adafruit_SleepyDog.h>
#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>
#include <Logger.h>
#include <Sensors/Loom_Analog/Loom_Analog.h>

/* CHANGE PARAMETERS! */
#define WISP_VERSION  2  // 1 or 2
#define WISP_INSTANCE 1  // Unit number
#define WISP_USE_LTE  1  // 0 or 1

// A packet is logged every 5 minutes, so mqtt will publish a batch of 72
// packets every 6 hours
#define WISP_BATCH_SIZE 72

// I2C device addresses used with multiplexer in V2
#define I2C_ADDR_DFGAS 0x74
#define I2C_ADDR_SEN66 0x6B
#define I2C_ADDR_SHT31 0x44

// MQTT credentials file on SD card
#define MQTT_CREDS_PATH "mqtt_creds.json"

#if WISP_USE_LTE
# include <Internet/Connectivity/Loom_LTE/Loom_LTE.h>
# include <Internet/Connectivity/Loom_Wifi/Loom_Wifi.h>
# include <Internet/Logging/Loom_MongoDB/Loom_MongoDB.h>
#endif  /* WISP_USE_LTE */

#if WISP_VERSION==1
# include <Sensors/I2C/Loom_SEN55/Loom_SEN55.h>
# include <Sensors/I2C/Loom_SHT31/Loom_SHT31.h>

#elif WISP_VERSION==2
# include <Hardware/Loom_Multiplexer/Loom_Multiplexer.h>
#endif  /* WISP_VERSION */

#define xstr(s) str(s)
#define str(s) #s

Manager manager("Wisp_V" xstr(WISP_VERSION) "_", WISP_INSTANCE);

Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST, true);

// Reads the battery voltage
Loom_Analog analog(manager);

#if WISP_USE_LTE
// 4G Connectivity
Loom_LTE lte(manager, "hologram", "", "");
Loom_BatchSD batchSD(hypnos, WISP_BATCH_SIZE);
Loom_MongoDB mqtt(manager, lte);
#endif  /* WISP_USE_LTE */

#if WISP_VERSION==1
// Main air quality, temperature, and humidity sensing
Loom_SEN55 SEN55(manager);
Loom_SHT31 sht(manager);

#elif WISP_VERSION==2
/* Initialize I2C sensors through multiplexer */
Loom_Multiplexer mux(manager, {I2C_ADDR_DFGAS, I2C_ADDR_SEN66, I2C_ADDR_SHT31});
#endif  /* WISP_VERSION */

void isrTrigger()
{
  hypnos.wakeup();
}

void setup() {

  ENABLE_SD_LOGGING;

  /* DISABLE FUNCTION SUMMARIES FOR FIELD DEPLOYMENT!
   * Function summaries are disabled to prevent excessive writing to SD card
   * as well as possible memory leak during deployment.
   * This issue may be fixed after merge with main, test later.
   */
  // ENABLE_FUNC_SUMMARIES;

  // Start the serial interface
  manager.beginSerial();

#if WISP_WISP_USE_LTE
  // Set the LTE board to only powerup when a batch is ready to be sent
  lte.setBatchSD(batchSD);
#endif  /* WISP_WISP_USE_LTE */

  // Both power rails should be on when awake
  hypnos.setWakeConfiguration(POWERRAIL_CONFIG::PR_3V_ON_5V_ON);

#if WISP_VERSION==1
  /* 5V rail should be on during sleep, 3V should be off
   * SEN55 (5V) requires multi-hour warm-up times, so the rail power cannot be
   * turned off during sleep.
   */
  hypnos.setSleepConfiguration(POWERRAIL_CONFIG::PR_3V_OFF_5V_ON);

#elif WISP_VERSION==2
  /* Both rails should be on during sleep.
   * DF MultiGas (5V) and SEN66 (3V) require multi-hour warm-up times, so the
   * rails power cannot be turned off during sleep.
   */
  hypnos.setSleepConfiguration(POWERRAIL_CONFIG::PR_3V_ON_5V_ON);

#endif  /* WISP_VERSION */

  // Enable the hypnos rails
  hypnos.enable();

#if WISP_USE_LTE
  // Time Sync Using LTE
  hypnos.setNetworkInterface(&lte);

  // Read the MQTT creds file to supply the device with MQTT credentials
  mqtt.loadConfigFromJSON(hypnos.readFile(MQTT_CREDS_PATH));
#endif  /* WISP_USE_LTE */

  // Initialize all modules
  // LTE initialization takes ~15 seconds, do this BEFORE starting the Watchdog
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);

#if WISP_USE_LTE
  hypnos.networkTimeUpdate();
#endif  /* WISP_USE_LTE */
}

void loop() {

  // Enable watchdog to prevent hang in measurement or logging
  Watchdog.enable(16000);
  Watchdog.reset();

  // Measure the data from the sensors
  manager.measure();

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

#if WISP_USE_LTE
  // Pass in the batchSD to the mqtt obj to check/ publish a batch of data if ready
  mqtt.publish(batchSD);
#endif  /* WISP_USE_LTE */

  // Set the interrupt duration for 5 minutes
  hypnos.setInterruptDuration(TimeSpan(0, 0, 5, 0));

  // Reattach the interrupt
  hypnos.reattachRTCInterrupt();

#if WISP_USE_LTE
  // Sync time (network updates can also block for several seconds)
  hypnos.networkTimeUpdate();
#endif  /* WISP_USE_LTE */

  // Set the hypnos to sleep, don't wait for user to open serial monitor
  hypnos.sleep(false);
}
