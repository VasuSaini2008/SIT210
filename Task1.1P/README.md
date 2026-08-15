# SIT210 Task 1.1P - Switching ON Lights

## Objective

The aim of this task is to make two lights work using an Arduino and a push button.

When the button is pressed, both lights turn on. The porch light turns off after 30 seconds and the hallway light turns off after 60 seconds.

## Components Used

- Arduino Nano 33 IoT
- Push button
- 2 LEDs
- Resistors
- Breadboard
- Jumper wires

## Pin Connections

| Component | Pin |
|---|---|
| Button | D2 |
| Porch LED | D3 |
| Hallway LED | D4 |

## How the Program Works

The Arduino first sets up the button and the two LEDs.

The button is connected to pin D2. The porch LED is connected to D3 and the hallway LED is connected to D4.

The Arduino keeps checking the button. When the button is pressed, both LEDs turn on.

The program waits for 30 seconds and then turns off the porch LED. It then waits for another 30 seconds and turns off the hallway LED.

## Timing

- Porch light: 30 seconds
- Hallway light: 60 seconds

## Modular Programming

I used the `setup()` and `loop()` functions to organise my program.

The `setup()` function is used to set the pins when the Arduino starts.

The `loop()` function keeps checking the button and controls what happens when the button is pressed.

This makes the program easier for me to understand and follow.

## Testing

I tested the circuit by pressing the button. Both LEDs turned on.

After 30 seconds, the porch LED turned off. After 60 seconds, the hallway LED also turned off.

The circuit worked as expected.

## Evidence

I will add screenshots of my circuit and testing here.
