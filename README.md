# Wisp
An open-sourced remote air quality sensing device made by OPEnS Lab OSU. The device logs air quality index parameters to the MongoDB database.


# Whisp | [Updates](https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/WeatherChimes-Updates) | [Github](https://github.com/OPEnSLab-OSU/Whisp)

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.7328528.svg)](https://doi.org/10.5281/zenodo.7328528)

Project leads: **Douglas Crocker**   -  crockerd@oregonstate.edu 

<p align="center">

<img src="https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/assets/105105069/ba47c487-7909-4c52-a4f4-3c9de111c7a2" width="600">
<p>


## Abstract
Whisp is an open-source air quality monitoring system, low-cost hardware and software suite that enables near real-time access to in-situ environmental sensor data (including particulate matter 1.0|2.5|4.0|10.0, volatile organic compounds, nitrogen oxides, temperature, and relative humidity) anywhere with a WiFi internet or cellular connection. Scientists, educators, and artists alike can use this tool to obtain and interact with environmental data in new and innovative ways, as well as collaborate remotely. Transforming data collection processes of environmental sensors into Internet of Things (IoT) compatible formats opens new doors into accessing, understanding, and interacting with natural phenomena. Whisp not only enables users to observe data online, but can also transform data into auditory signals and soundscapes through sonification processes or creative animations using newly-created computer applications.


| Specification |  Sensor       | Resolution   | | Accuracy      |               |Full Range     |   |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |------------- |------------- |
|  | | **Value**|**Metric**| **Value** | **Metric**|  **Value** | **Metric**|
| Senses Ambient Temperature| SHT30| 0.01| ℃| ±0.3 | ℃| -40 - 125 |℃|
| Senses Humidity |SHT30| 0.015|%RH| ±3|%RH|  0 - 100 |%RH|
| Particulate Matter 1.0 |SEN55| ±5 |ug/m^3| ±5|ug/m^3 | 0 - 1000|ug/m^3|
| Particulate Matter 2.5 |SEN55| ±5 |ug/m^3| ±5|ug/m^3 | 0 - 1000|ug/m^3|
| Particulate Matter 4 |SEN55| ±25 |ug/m^3| ±25|ug/m^3 | 0 - 1000|ug/m^3|
| Particulate Matter 10 |SEN55| ±25 |ug/m^3| ±25|ug/m^3 | 0 - 1000|ug/m^3|
| Volatile Organic Compounds |SEN55| ±5 |VOC index points| ±5| VOC index points |1 - 500 |VOC index points|
| Nitrogen Oxides Index |SEN55| ±10 |NOx index points| ±10| NOx index points |1 - 500 |NOx index points|
| Senses Pressure   |MS580302|30 - 110|kPa|±0.15|kPa
| 6 Watt Solar Panel|        |||||||
| Data Collection Frequency | |  | | ||5| Minutes|
| Battery life (Up to 3 10050 mAh batteries)	|  |  |||| 25 | Days|
| Project Cost |  |  |  |||<800 | $|
| Logs Data to SD |  |  | | ||Time | date/hour/min |	
| SD and USB are easily accessible |		




## Hardware In Context
Each *Whisp* can measure Particulate Matter 10.0|4.0|2.5|1.0, Volatile Organic Compounds(VOC), and nitrogen Oxides(NOx) (SEN55); and air temperature and humidity (SHT31/SHT30), and log data at user defined intervals to the cloud database: MongoDB. Beyond the sensors used in this paper, the *Whisp* is capable of using a variety of analog, digital, I2C, SDI-12, and other serial sensors via footprints on the Printed Circuit Board (PCB) detailed in the sections below. While many other sensors like rainfall, air quality, and wind direction could have been chosen, we selected the current combination of sensors to fulfill a demand that existed within a local agricultural research lab. The Whisp can operate for up to 27 days on a battery capacity of 10050 mAh with a logging period of every ten minutes. The logging period is arbitrary and can be adjusted to accommodate any power requirements. The total operation duration of the system can be lengthened significantly with the addition of a solar panel and better power management, which is recommended in areas with lack of access to a dedicated power source. . 
\
The integration of particulate matter data into a centralized cloud database by the Whisp unit enables the aggregation and analysis of air quality data on a broader scale. By centralizing this data, researchers can more effectively identify trends and patterns in air quality over time. This approach not only facilitates the detection of emerging environmental trends but also enhances the understanding of the impact of various factors on air quality.

\
The Whisp device produced at the OPEnS Lab aims to collect environmental data. Data is streamed in real time via a local server to MongoDB, an online database. Computer applications can subscribe to these data stream feeds and data analysis can be conducted based 


## Hardware Description
**Version 1**
\
The Pelican case has three holes drilled on the side to accommodate for the PG7 cable glands and waterproof cable set. This allows for the [SHT30](https://www.adafruit.com/product/4099) and [GS3](http://library.metergroup.com/Manuals/20429_GS3_Web.pdf) sensor to be swapped out easily. Inside the case, a custom 3D printed base plate holds the Featherwing doubler, LTE cellular board,  and batteries securely in place. A Feather M0 WiFi and [Hypnos](https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos) v3.3 board are used to store data collected by a particulate matter sensor (SEN55) and temperature & humidity sensor (SHT30). The v3.3 Hypnos board turns peripherals on and off to preserve power, wake up at intervals using the embedded DS3231 RTC, transmit data via cellular LTE and store data onboard a microSD card. In order to enable 4G capabilities the use of components such as the [SARA-R4 4G board](https://www.sparkfun.com/products/14997) for 4G cellular connectivity, solar charger and 6 Watt solar panel is implemented. 

<p align="center">
<img width="361" alt="Whisp v0 brd Picture" src="https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/assets/105105069/7bdb389b-5b91-42b9-ae72-26ed21451b91" width = "400">

<p>

<p align = "center">
Figure 1: Whisp v1 PCB with footprints for analog, digital, I2C, and other serial sensors
</p>

 Other I2C sensors may also be connected as long as there is relevant code to handle requesting data on the Feather M0.


<p>&nbsp;</p>
<p align="center">
  <img src="https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/assets/105105069/f0d42f72-34d2-4a11-b81a-b97e0d22e1aa" width="600" > 

</p>
<p align="center">
Figure 2: Fully built Whisp device 

## Electronics

<p align="center">
<img width="500" alt="image" src="https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/assets/105105069/00871e87-b122-4b99-ac3b-c0f6e6f89bd9">

<p>

<p align = "center">
Figure 3: Block diagram of WeatherChimes electronics
</p>

The most relevant features of the electronics system are the following:
* Measures Particulate Matter 1.0/2.5/5.0/10.0, volatile organic compounds, and nitrogen oxides
* Measures air temperature and humidity
* Saves data to SD
* Onboard RTC and power switching relays for power savings
* Cellular LTE access to upload data to the MongoDB server
* WiFi access to upload data to MongoDB server



## Current Draw Test
<p align="center">
<img src="https://user-images.githubusercontent.com/86391366/167509252-332a4d7e-60e6-4997-b2ec-00b88ba90b25.png" width="600"> 
<p>

The Chime device draws 107.5 mA when initializing and 108.9 mA during WiFi. It sleeps for 10 minutes in-between data cycles and uses 14.8 mA. When running on an 10050 mA battery, the device will approximately last 27.32 days.

## State Machine Diagram

Each sample cycle is triggered by RTC alarm to wake from a low-power sleep mode, the Feather M0 requests data from each of the sensors with the Loom Measure code and formats the data according to each logging platform: comma separated for local storage on microSD and JSON for telemetry. After all sensor information has been collected and formatted, the Feather will initiate a message over WiFi to a remote MQTT (Message Queueing Telemetry Transport) broker which is being run on an OSU server. 


<img width="879" alt="image" src="https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/assets/105105069/84997534-468d-45ce-ac49-34dc9aa48742">


<p align = "center">
Figure 4: Data-flow handling chart
</p>

MQTT brokers work by utilizing a publish/subscribe paradigm, this paradigm works on the basis that there are “topics” that are public to everyone viewing the broker. Users can subscribe to topics which allows them to receive a callback when new data is published to the topic. For Whisp, all data messages are sent over a topic, the topic is formatted with the “Site Name”/”Device Name” + “Device Number”  to distinguish between the devices and their locations and determine the destination, i.e. collection, in the MongoDB database. Assigning a two part topic to each message allows multiple devices, even with the same name, to publish to different collections of data. 


## Resource List

* [Bill of Materials](https://docs.google.com/spreadsheets/d/13oa5u93MhcqooUcmy1VRU9iUPKT63fNZ0G9CzINLz3U/edit?usp=sharing)

* [Build Guide](https://docs.google.com/document/d/1GEz6TniiCkyVJEQ1pW2CY4VUsa4j7f_cYcETQBzS96c/edit?usp=sharing)

* [Power Budget](https://docs.google.com/spreadsheets/d/1JvEoA3bKAHnuwdlADwK3NVbHGGMLVKzisJuQMVjQb0Y/edit?usp=sharing)

* [Whisp PCB v0.zip](https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/files/14748869/Whisp.PCB.v0.zip)


* [Loom V4 Repository](https://github.com/OPEnSLab-OSU/Loom-V4)


## Tutorials

*  [MongoDB manual](https://docs.mongodb.com/manual/)



