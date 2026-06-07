# La-casa-IoT-de-papel (SmartHome)
## It's my first solo IoT project and it designs an IoT system that acts as a smart home in a simulated cardboard house. I used an ESP32 development board and multiple sensors connected through a breadboard.

The system monitors:
* temperature and humidity,
* motion detection,
* air quality / harmful gases,
* light level,
* soil moisture.

Additionally, the project controls:
* a cooling fan using a relay module,
* LEDs, that simulate lamps across the whole house
* an alarm buzzer.

## Hardware Used
### Main Controller

* ESP32 DevKit V1

### Sensors
* DHT22 temperature and humidity sensor
* HC-SR505 PIR motion sensor
* MQ-2 gas sensor
* HW-077 light sensor module
* Capacitive Soil Moisture Sensor v2.0

### Output Components

* LED diodes
* Active buzzer module
* 5V cooling fan
* Relay module

### Other Components

* Breadboard
* Jumper wires
* Resistors

## Features

* Real-time temperature and humidity monitoring
* Motion-triggered alarm system
* Harmful gas detection
* Automatic LED activation in darkness
* Soil moisture monitoring
* Automatic cooling fan activation when temperature exceeds a threshold
* WiFi connection using ESP32
* HTTP API endpoints for reading sensor data and controlling outputs remotely


### I managed to learn a lot. This project showed me:
How GPIO pins work on ESP32 and how to handle unstable GPIO states,
Differences between digital and analog sensors,
How to use pull-up and pull-down logic,
How relays work
How to build HTTP APIs on ESP32
How to manage power distribution and wired connections
How to debug many hardware and software issues that occured during development.

This entire prototype was built using breadboard and jumper wires, so for the future projects I'm planning to use soldering iron to improve cable managment and hardware reliability.


