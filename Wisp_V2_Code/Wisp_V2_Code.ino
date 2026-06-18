#include <Loom_Manager.h>
#include <Adafruit_SleepyDog.h>
#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>
#include <Hardware/Loom_Multiplexer/Loom_Multiplexer.h>
#include <Internet/Connectivity/Loom_LTE/Loom_LTE.h>
#include <Internet/Connectivity/Loom_Wifi/Loom_Wifi.h>
#include <Internet/Logging/Loom_MongoDB/Loom_MongoDB.h>
#include <Logger.h>
#include <Sensors/Loom_Analog/Loom_Analog.h>

/* CHANGE INSTANCE NUMBER! */
Manager manager("Wisp_V2_", 1);

Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST, true);

// 4G Connectivity
Loom_LTE lte(manager, "hologram", "", "");

// A batch is logged every 5 minutes, so mqtt will publish a batch of 72 every 6 hours
Loom_BatchSD batchSD(hypnos, 72);
Loom_MongoDB mqtt(manager, lte);

// Reads the battery voltage
Loom_Analog analog(manager);

/* Initialize I2C sensors through multiplexer
 * 0x74 - DF MultiGas (3x)
 * 0x6B - SEN66
 * 0x44 - SHT31
 */
Loom_Multiplexer mux(manager, {0x74, 0x6B, 0x44});

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

  // Set the LTE board to only powerup when a batch is ready to be sent
  lte.setBatchSD(batchSD);

  // Both power rails should be on when awake
  hypnos.setWakeConfiguration(POWERRAIL_CONFIG::PR_3V_ON_5V_ON);

  /* Both rails should be on during sleep.
   * DF MultiGas (5V) and SEN66 (3V) require multi-hour warm-up times, so the
   * rails power cannot be turned off during sleep.
   */
  hypnos.setSleepConfiguration(POWERRAIL_CONFIG::PR_3V_ON_5V_ON);

  // Enable the hypnos rails
  hypnos.enable();

  // Time Sync Using LTE
  hypnos.setNetworkInterface(&lte);

  // Read the MQTT creds file to supply the device with MQTT credentials
  mqtt.loadConfigFromJSON(hypnos.readFile("mqtt_creds.json"));

  // Initialize all modules
  // LTE initialization takes ~15 seconds, do this BEFORE starting the Watchdog
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);

  hypnos.networkTimeUpdate();
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

  // Pass in the batchSD to the mqtt obj to check/ publish a batch of data if ready
  mqtt.publish(batchSD);

  // Set the interrupt duration for 5 minutes
  hypnos.setInterruptDuration(TimeSpan(0, 0, 5, 0));

  // Reattach the interrupt
  hypnos.reattachRTCInterrupt();

  // Sync time (network updates can also block for several seconds)
  hypnos.networkTimeUpdate();

  // Set the hypnos to sleep with power still supplied to both rails
  // Don't wait for user to open serial monitor
  hypnos.sleep(false);
}
