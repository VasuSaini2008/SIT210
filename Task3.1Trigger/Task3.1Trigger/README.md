# Task 3.1P - Trigger and Notification

## Project
This project uses an Arduino Nano 33 IoT and BH1750 light sensor
to measure the amount of light received by a terrarium.

## Hardware
- Arduino Nano 33 IoT
- BH1750 light sensor
- Breadboard
- Jumper wires

## Software
- Arduino IDE
- MQTT / Mosquitto
- Node-RED

## How it works
The BH1750 measures the light level in lux.

The Arduino reads the light level and publishes the value
to the MQTT topic:

terrarium/light

Node-RED receives the MQTT message and checks the light level.
A notification is generated when the sunlight condition changes.

## MQTT
Broker: 192.168.1.5
Port: 1883
Topic: terrarium/light
