# La-casa-IoT-de-papel--SmartHome-
###It's my first solo IoT project and it designs an IoT system that acts as a smart home in a simulated cardboard house. I used an ESP32 development board and multiple sensors connected through a breadboard.

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
* USB power supply

## Features

* Real-time temperature and humidity monitoring
* Motion-triggered alarm system
* Harmful gas detection
* Automatic LED activation in darkness
* Soil moisture monitoring
* Automatic cooling fan activation when temperature exceeds a threshold
* WiFi connection using ESP32
* HTTP API endpoint for reading sensor data remotely

## Software

The project was programmed in:

* Arduino IDE
* C++

The ESP32 hosts a simple HTTP server that exposes sensor data in JSON format.

Example endpoint:

```json
{
  "temperature": 24.3,
  "humidity": 51.8,
  "soil": 2780
}
```

## What I Learned

During this project I learned:

* how GPIO pins work on ESP32,
* differences between digital and analog sensors,
* how to use pull-up and pull-down logic,
* how relays work,
* how to debug hardware problems,
* how to connect sensors using a breadboard,
* how to build HTTP APIs on ESP32,
* basics of embedded systems and IoT development,
* how power distribution works in electronic systems.

I also learned how important proper wiring and voltage management are, especially when mixing 3.3V and 5V devices.

## Challenges

Some of the biggest challenges included:

* debugging relay behavior,
* handling unstable GPIO states,
* troubleshooting incorrect VCC/GND connections,
* managing power consumption from multiple sensors,
* understanding how PIR and MQ-2 sensors behave during calibration.

## Future Improvements

This entire prototype was built only using:

* a breadboard,
* jumper wires,
* temporary connections.

In the next version of the project I plan to:

* solder the components permanently,
* improve cable management,
* use external power supplies,
* add more actuators and sensors,
* create a web dashboard or mobile app,
* improve enclosure and hardware reliability.

## Summary

This project was my first larger ESP32 and embedded systems project. It helped me understand both software and hardware aspects of IoT development, including sensor integration, automation logic, networking, and electronics debugging.
