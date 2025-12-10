/**
 * Uses the I2C Multiplexer to dynamically allow hot swapping of I2C sensors
 * 
 * MANAGER MUST BE INCLUDED FIRST IN ALL CODE
 */


#include <Loom_Manager.h>

#include <Hardware/Loom_Multiplexer/Loom_Multiplexer.h>
#include <Sensors/Loom_Analog/Loom_Analog.h>

#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>
#include <Internet/Connectivity/Loom_LTE/Loom_LTE.h>

#include <Sensors/I2C/Loom_T6793/Loom_T6793.h>

#include <Logger.h>
#include <Internet/Connectivity/Loom_Wifi/Loom_Wifi.h>
#include <Internet/Logging/Loom_MongoDB/Loom_MongoDB.h>

Manager manager("Multiplex_Gas_PCBTest", 1);

Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST, true);

Loom_LTE lte(manager, "hologram", "", "");
Loom_MongoDB mqtt(manager, lte);

Loom_Analog analog(manager);

//A batch is logged every 5 minutes, so 12 per hour (12 * 6 = 72) so mqtt will publish at batch size of 72/ every 6 hours
Loom_BatchSD batchSD(hypnos, 72);


//Loom_DFMultiGasSensor: 0x74



// Reads the battery voltage
// Loom_Multiplexer mux(manager, {0x74, 0x15, 0x69});
// Loom_Multiplexer mux(manager, {0x15, 0x69});

// Loom_Multiplexer mux(manager, {0x74});
// Loom_Multiplexer mux(manager , {0x74, 0x15, 0x69});
Loom_Multiplexer mux(manager , {0x74, 0x15, 0x69, 0x44});



// Loom_T6793 T6793(manager);

void isrTrigger()
{
  hypnos.wakeup();
}

void setup() {

  ENABLE_SD_LOGGING;
  // ENABLE_FUNC_SUMMARIES;

  // Start the serial interface
  manager.beginSerial();

  // Set the LTE board to only powerup when a batch is ready to be sent
  lte.setBatchSD(batchSD);

  // Both power rails should be on when awake
  hypnos.setWakeConfiguration(POWERRAIL_CONFIG::PR_3V_ON_5V_ON);

  // Only the 5V rail should be on during sleep
  hypnos.setSleepConfiguration(POWERRAIL_CONFIG::PR_3V_OFF_5V_ON);

  // Enable the hypnos rails
  hypnos.enable();

  //Time Sync Using LTE 
  hypnos.setNetworkInterface(&lte);

  // Read the MQTT creds file to supply the device with MQTT credentials
  mqtt.loadConfigFromJSON(hypnos.readFile("mqtt_creds.json"));

  // Initialize the manager
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);

  hypnos.networkTimeUpdate();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Measure the data from the sensors
  manager.measure();

  // Package the data into JSON
  manager.package();
  
  // Log the data to the SD
  hypnos.logToSD();

  // Pass in the batchSD to the mqtt obj to check/ publish a batch of data if ready
  mqtt.publish(batchSD);

  // Print the JSON document to the Serial monitor
  manager.display_data();

  // Set the interrupt duration for 5 minutes
  hypnos.setInterruptDuration(TimeSpan(0,0,5,0));

  // Reattach the interrupt
  hypnos.reattachRTCInterrupt();

  // Set the hypnos to sleep, but with power still being supplied to the 5v rail (wait for serial when testing from a computer)

  hypnos.networkTimeUpdate();
  hypnos.sleep();

  // Wait for 5 seconds
  // manager.pause(5000);
}