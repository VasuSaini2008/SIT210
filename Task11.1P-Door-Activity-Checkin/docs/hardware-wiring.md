# Hardware Wiring Information

## Arduino Nano 33 IoT pin map

| Component | Signal | Arduino pin |
|---|---|---:|
| DHT22 module | DATA | D9 |
| PIR module | OUT | D3 |
| HC-SR04 | TRIG | D4 |
| HC-SR04 | ECHO | D5 (through voltage divider) |
| Push button | Input to GND (INPUT_PULLUP) | D6 |
| Green LED | Anode through resistor | D7 |
| Red LED | Anode through resistor | D8 |
| Blue LED | Anode through resistor | D10 |
| Yellow LED | Anode through resistor | D11 |
| Buzzer driver | Driver control input | D12 |

## Safety and wiring notes

- Connect sensor grounds and Arduino ground together.
- The push button uses `INPUT_PULLUP`; connect it between D6 and GND.
- Give each LED its own current-limiting resistor (typically 220–330 ohms).
- Power the DHT22 module according to its board specifications. The discussed prototype wiring used 3.3V.
- The HC-SR04 is normally powered from 5V. Its ECHO output may be 5V, which is not safe for the Nano 33 IoT's 3.3V GPIO. Use a voltage divider or level shifter before D5. One example divider is 1 kΩ from ECHO to D5 and 2 kΩ from D5 to GND.
- Check the PIR module output voltage before connecting it to the 3.3V Arduino input. Use level shifting if its output can exceed 3.3V.
- D12 is a control signal for a suitable driver stage. Do not power an unknown buzzer directly from an Arduino GPIO. Confirm buzzer voltage, current and driver polarity before connecting.
- Verify wiring and power requirements before switching on the circuit.

## Raspberry Pi network

Find the Pi's current LAN address using:

```bash
hostname -I
```

Set that address in the Arduino sketch. It may change if the router assigns a different DHCP lease.
