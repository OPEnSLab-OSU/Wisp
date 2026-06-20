#include <Loom_Manager.h>
#include <Adafruit_SleepyDog.h>
#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>
#include <Logger.h>
#include <Sensors/Loom_Analog/Loom_Analog.h>
#include <Sensors/I2C/Loom_SEN55/Loom_SEN55.h>
#include <Sensors/I2C/Loom_SHT31/Loom_SHT31.h>

/* CHANGE INSTANCE NUMBER! */
Manager manager("Whisp_brd_v0p4_", 1);

Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST, true);

// Reads the battery voltage
Loom_Analog analog(manager);

// Main air quality, temperature, and humidity sensing
Loom_SEN55 SEN55(manager);
Loom_SHT31 sht(manager);

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

  /* 5V rail should be on during sleep, 3V should be off
   * SEN55 (5V) requires multi-hour warm-up times, so the rail power cannot be
   * turned off during sleep.
   */
  hypnos.setSleepConfiguration(POWERRAIL_CONFIG::PR_3V_OFF_5V_ON);

  // Enable the hypnos rails
  hypnos.enable();

  // Initialize all modules
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);
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

  // Set the interrupt duration for 5 minutes
  hypnos.setInterruptDuration(TimeSpan(0, 0, 5, 0));

  // Reattach the interrupt
  hypnos.reattachRTCInterrupt();

  // Set the hypnos to sleep with power still supplied to the 5v rail
  // Don't wait for user to open serial monitor
  hypnos.sleep(false);
}
