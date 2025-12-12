# Wisp
An open-sourced remote air quality sensing device made by OPEnS Lab OSU. The device logs air quality index parameters to the MongoDB database.


# Wisp | [Updates](https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/WeatherChimes-Updates) | [Github](https://github.com/OPEnSLab-OSU/Wisp)


Project leads: **Soren Emmons** - emmonsso@oregonstate.edu, **Douglas Crocker**   -  crockerd@oregonstate.edu 

<p align="center">
<img width="806" height="853" alt="image" src="https://github.com/user-attachments/assets/7135006f-c615-40a3-88c8-dcc87a8b4813" />




## Abstract
Wisp is an open-source air quality monitoring system, a low-cost hardware and software suite that enables near real-time access to in-situ environmental sensor data (including particulate matter 1.0|2.5|4.0|10.0, volatile organic compounds, nitrogen oxides, temperature, and relative humidity) anywhere with a WiFi internet or cellular connection. Scientists, educators, and artists alike can use this tool to obtain and interact with environmental data in new and innovative ways, as well as collaborate remotely. Transforming data collection processes of environmental sensors into Internet of Things (IoT) compatible formats opens new doors into accessing, understanding, and interacting with natural phenomena. Wisp not only enables users to observe data online, but can also transform data into auditory signals and soundscapes through sonification processes or creative animations using newly-created computer applications.

## Wisp V1 Sensor Specs

| Specification |  Sensor       | Resolution   | | Accuracy      |               |Full Range     |   |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |------------- |------------- |
|  | | **Value**|**Metric**| **Value** | **Metric**|  **Value** | **Metric**|
| Senses Ambient Temperature| SHT31| 0.01| ℃| ±0.3 | ℃| -40 - 125 |℃|
| Senses Humidity |SHT31| 0.015|%RH| ±3|%RH|  0 - 100 |%RH|
| Particulate Matter 1.0 |SEN55| ±5 |ug/m^3| ±5|ug/m^3 | 0 - 1000|ug/m^3|
| Particulate Matter 2.5 |SEN55| ±5 |ug/m^3| ±5|ug/m^3 | 0 - 1000|ug/m^3|
| Particulate Matter 4 |SEN55| ±25 |ug/m^3| ±25|ug/m^3 | 0 - 1000|ug/m^3|
| Particulate Matter 10 |SEN55| ±25 |ug/m^3| ±25|ug/m^3 | 0 - 1000|ug/m^3|
| Volatile Organic Compounds |SEN55| ±5 |VOC index points| ±5| VOC index points |1 - 500 |VOC index points|
| Nitrogen Oxides Index |SEN55| ±10 |NOx index points| ±10| NOx index points |1 - 500 |NOx index points|
| 5 Watt Solar Panel|        |||||||
| Data Collection Frequency | |  | | ||5| Minutes|
| Battery life (Up to 3 10050 mAh batteries)	|  |  |||| 25 | Days|
| Project Cost |  |  |  |||<800 | $|
| Logs Data to SD |  |  | | ||Time | date/hour/min |	
| SD and USB are easily accessible |		




## Hardware In Context
Each *Wisp* can measure Particulate Matter 10.0|4.0|2.5|1.0, Volatile Organic Compounds(VOC), and nitrogen Oxides(NOx) (SEN55); and air temperature and humidity (SHT31/SHT30), and log data at user-defined intervals to the cloud database: MongoDB. Beyond the sensors used in this paper, the *Wisp* is capable of using a variety of analog, digital, I2C, SDI-12, and other serial sensors via footprints on the Printed Circuit Board (PCB) detailed in the sections below. While many other sensors, like rainfall, air quality, and wind direction, could have been chosen, we selected the current combination of sensors to fulfill a demand that existed within a local agricultural research lab. The Wisp can operate for up to a month on a battery capacity of 3 10050 mAh battereies with a logging period of every five minutes. The logging period is arbitrary and can be adjusted to accommodate any power requirements. The total operation duration of the system can be lengthened significantly with the addition of a solar panel and better power management, which is recommended in areas with lack of access to a dedicated power source. 
\
The integration of particulate matter data into a centralized cloud database by the Wisp unit enables the aggregation and analysis of air quality data on a broader scale. By centralizing this data, researchers can more effectively identify trends and patterns in air quality over time. This approach not only facilitates the detection of emerging environmental trends but also enhances the understanding of the impact of various factors on air quality.

\
The Wisp device produced at the OPEnS Lab aims to collect environmental data. Data is streamed in real time via a local server to MongoDB, an online database. Computer applications can subscribe to these data stream feeds, and data analysis can be conducted based on the data sent by Wisp units.  


## Hardware Description
**Version 1**
\
The Pelican case has three holes drilled on the side to accommodate the PG7 cable glands and waterproof cable set. This allows for the [SHT31](https://www.digikey.com/en/products/detail/dfrobot/SEN0385/13590873?gclsrc=aw.ds&gad_source=1&gad_campaignid=20232005509&gbraid=0AAAAADrbLlgN9gdgD9ae58ipF8U3-kqXP&gclid=CjwKCAiAl-_JBhBjEiwAn3rN7ZY7lfRuItuFSmiqk9HOFaHFD5hMuSXhRHAJTuwCGXXSsgWjr_exEBoC_ecQAvD_BwE) and [SEN55](https://www.digikey.com/en/products/detail/sensirion-ag/SEN55-SDN-T/16342756?gclsrc=aw.ds&gad_source=1&gad_campaignid=20232005509&gbraid=0AAAAADrbLlgN9gdgD9ae58ipF8U3-kqXP&gclid=CjwKCAiAl-_JBhBjEiwAn3rN7R_zAgQmUyPsM8nMsHZEcr-VeQwOGRPi851icy3Jx-7ERo_M-4K_oRoCu1gQAvD_BwE) sensor to be swapped out easily. Inside the case, a custom 3D printed base plate holds the Featherwing doubler, LTE cellular board, and batteries securely in place. A Feather M0 WiFi and [Hypnos](https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos) v3.3 board is used to store data collected by a particulate matter sensor (SEN55) and temperature & humidity sensor (SHT31). The v3.3 Hypnos board turns peripherals on and off to preserve power, wakes up at intervals using the embedded DS3231 RTC, transmits data via cellular LTE, and stores data onboard a microSD card. In order to enable 4G capabilities, the use of components such as the [SARA-R4 4G board](https://www.sparkfun.com/products/14997) for 4G cellular connectivity, a solar charger, and a 5 Watt solar panel is implemented. 


<p align="center">
<img width="867" height="912" alt="image" src="https://github.com/user-attachments/assets/1b245f23-9401-431b-ad87-d6cbec88cc81" />

<p>

<p align = "center">
Figure 1: Wisp v1 PCB with footprints for analog, digital, I2C, and other serial sensors
</p>
<p align="center">
 Other I2C sensors may also be connected as long as there is relevant code to handle requesting data on the Feather M0.
</p>

<p>&nbsp;</p>
<p align="center">

  
<img width="1564" height="1038" alt="image" src="https://github.com/user-attachments/assets/e75312ab-8188-4374-b316-18e5ab308005" />

</p>
<p align="center">
Figure 2: Fully built Wisp device 

## Electronics

<p align="center">
<img width="500" alt="image" src="https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/assets/105105069/00871e87-b122-4b99-ac3b-c0f6e6f89bd9">

<p>

<p align = "center">
Figure 3: Block diagram of Wisp electronics
</p>

The most relevant features of the electronics system are the following:
* Measures Particulate Matter 1.0/2.5/5.0/10.0, volatile organic compounds, and nitrogen oxides
* Measures air temperature and humidity
* Saves data to SD
* Onboard RTC and power switching relays for power savings
* Cellular LTE access to upload data to the MongoDB server
* WiFi access to upload data to MongoDB server



## Current Draw Test

The Wisp device draws approximately 20mA when initializing and 117mA during sensor polling. During transmission, the Wisp draws 305mA peak current. It sleeps for 5 minutes between data cycles, draws a nominal 5 mA, and peaks at 30 mA using just the battery. A Wisp can operate for approximately one month, transmitting every 6 hours using 5-minute sleep intervals. 

## State Machine Diagram

Each sample cycle is triggered by RTC alarm to wake from a low-power sleep mode, the Feather M0 requests data from each of the sensors with the Loom Measure code and formats the data according to each logging platform: comma separated for local storage on microSD and JSON for telemetry. After all sensor information has been collected and formatted, the Feather will initiate a message over 4G to a remote MQTT (Message Queueing Telemetry Transport) broker which is being run on an OSU server. 


<p align = "center">
<img width="879" alt="image" src="https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/assets/105105069/84997534-468d-45ce-ac49-34dc9aa48742">
</p>

<p align = "center">
Figure 4: Data-flow handling chart
</p>

MQTT brokers work by utilizing a publish/subscribe paradigm, this paradigm works on the basis that there are “topics” that are public to everyone viewing the broker. Users can subscribe to topics which allows them to receive a callback when new data is published to the topic. For Wisp, all data messages are sent over a topic, the topic is formatted with the “Site Name”/”Device Name” + “Device Number”  to distinguish between the devices and their locations and determine the destination, i.e. collection, in the MongoDB database. Assigning a two part topic to each message allows multiple devices, even with the same name, to publish to different collections of data. 

## Deployment History
Increasing wildfire frequency and intensity across California, Oregon, and Washington pose a significant threat to the wine industry through the phenomenon of smoke taint, where volatile phenols from smoke are absorbed by grapes, negatively impacting wine quality. So for the past two years, Wisp has been deployed across the West Coast in order to collect data on smoke particualtes in vineyards. Over this past year, OPEnS has handled 43 Wisp deployments at vineyards where their data is currently being used by UC Davis, OSU, and WSU.

<p align = "center">
<img width="900" height="500" alt="image" src="https://github.com/user-attachments/assets/4799d9f0-c3be-4c77-b260-e1e982711efe" />
</p>

<p align = "center">
Figure #: PM 2.5 graph from 6/25/25 - 9/3/25 in Washington
</p>

<p align = "center">
<img width="900" height="500" alt="image" src="https://github.com/user-attachments/assets/b0d793c7-6f7e-4e41-ac06-b3f074bc1c03" />
</p>

<p align = "center">
Figure #: VOC index graph from 6/25/25 - 9/3/25 in Washington
</p>

<p align = "center">
<img width="2469" height="508" alt="image" src="https://github.com/user-attachments/assets/f3586e3b-fb2d-44d4-baf8-bf03e1f2033f" />
</p>

<p align = "center">
Figure #: CSV File output from Wisp unit
</p>
 
 
## Wisp V2
</p>

Wisp V2, is an updated version of the Wisp sensor supporting 4 DFRobot [SEN0465 to SEN0476](https://www.dfrobot.com/product-2510.html) line of gas sensors and the [T6793-5k](https://www.digikey.com/en/products/detail/amphenol-advanced-sensors/T6793-5K/16368385) CO2 sensor. The [SEN0465 to SEN0476] line of sensors are all supported by the Wisp V2 allowing the user to choose the types of gases that fit their use case. The new Wisp V2 board, now has a I2C multiplexer helping with stability with the increased number of sensors and allowing for a more modular device. All of the sensors are now integrated into the new fully 3D printed waterproof Wisp case that also allows for easier user access of the SD card as well allowing for up to five 1sLipo 10050mAh batteries. The goal with this new version of the Wisp is to expand the units sensing capabilities while still being reliable and modular to fit many use cases and research projects. The Wisp V2 is currently undergoing testing with prototype units but is expected to begin being deployed for field data collection in May of 2026. 

<table>
  <tr>
    <td width="50%">
      <h3 align="center"></h3>
      <div align="center">
        <img width="1067" height="800" alt="image" src="https://github.com/user-attachments/assets/62591cfc-5b2d-4233-a8a0-235041e55f9a" />
       </p>
      </div>
      <p align="center">
      </p>
      </p>
    </td>
    <td width="50%">
      <h3 align="center"></h3>
      <div align="center">
        <img src="https://github.com/user-attachments/assets/99ca0bdf-7a26-4a01-a52e-a6c57e7c323d">
      </div>
      <p align="center">
      </p>
      </p>
    </td>
  </tr>
</table>
</p>
<p align="center">
Figure #: 3D Renders of the Wisp V2
</p>
<p align="center">
<img width="700" height="800" alt="image" src="https://github.com/user-attachments/assets/8ba0f92d-618b-4d27-87f9-11aa199c0dd8" />
</p>
<p align="center">
Figure #: Wisp V2 PCB
</p>
<div align="center">

## Wisp V2 Sensor Specs

|  **Specification**  |  **Sensor**  |  **Resolution**  |  **Accuracy**  |  **Full Range**  |
| :--- | :---: | :---: | :---: | :---: |
| Ambient Temperature | SHT31 | 0.01 ℃ | ±0.3 ℃ | -40 to 125 ℃ |
| Humidity | SHT31 | 0.015 %RH | ±3 %RH | 0 to 100 %RH |
| Particulate Matter 1.0 | SEN55 | ±5 µg/m³ | ±5 µg/m³ | 0 to 1000 µg/m³ |
| Particulate Matter 2.5 | SEN55 | ±5 µg/m³ | ±5 µg/m³ | 0 to 1000 µg/m³ |
| Particulate Matter 4.0 | SEN55 | ±25 µg/m³ | ±25 µg/m³ | 0 to 1000 µg/m³ |
| Particulate Matter 10 | SEN55 | ±25 µg/m³ | ±25 µg/m³ | 0 to 1000 µg/m³ |
| Volatile Organic Compounds | SEN55 | ±5 Index | ±5 Index | 1 to 500 Index |
| Nitrogen Oxides Index | SEN55 | ±10 Index | ±10 Index | 1 to 500 Index |
| Sulfur Dioxide (SO2) | SEN0470 | 0.1 ppm | ±10% | 0 to 20 ppm |
| Carbon Monoxide (CO) | SEN0466 | 1 ppm | ±10% | 0 to 1000 ppm |
| Ozone (O3) | SEN0472 | 0.1 ppm | ±10% | 0 to 10 ppm |
| Hydrogen Sulfide (H2S) | SEN0467 | 1 ppm | ±10% | 0 to 100 ppm |
| Carbon Dioxide (CO2) | T6793-5K | 1 ppm | ±45 ppm | 400 to 5000 ppm |

 


<p align = "center"> 
 
## Resource List

* [Bill of Materials](https://docs.google.com/spreadsheets/d/13oa5u93MhcqooUcmy1VRU9iUPKT63fNZ0G9CzINLz3U/edit?usp=sharing)

* [Build Guide](https://docs.google.com/document/d/1GEz6TniiCkyVJEQ1pW2CY4VUsa4j7f_cYcETQBzS96c/edit?usp=sharing)

* [Power Budget](https://docs.google.com/spreadsheets/d/1JvEoA3bKAHnuwdlADwK3NVbHGGMLVKzisJuQMVjQb0Y/edit?usp=sharing)

* [Wisp PCB v0.zip](https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/files/14748869/Wisp.PCB.v0.zip)


* [Loom V4 Repository](https://github.com/OPEnSLab-OSU/Loom-V4)


## Tutorials

*  [MongoDB manual](https://docs.mongodb.com/manual/)



