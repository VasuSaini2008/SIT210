# Door and Activity Check-in System

SIT210 Embedded Systems Development — Task 11.1P Project Demo and Artefacts.

This folder documents a prototype built with an Arduino Nano 33 IoT and a Raspberry Pi 4. The Arduino reads sensors and publishes readings over Wi-Fi using MQTT. The Raspberry Pi runs Mosquitto and a Python Flask dashboard.

## Architecture

Sensors and button → Arduino Nano 33 IoT → Wi-Fi / MQTT → Raspberry Pi 4 (Mosquitto) → Python Flask dashboard → browser.

## Hardware used

- Arduino Nano 33 IoT
- DHT22 temperature and humidity module
- HC-SR501 PIR motion sensor
- HC-SR04 ultrasonic distance sensor
- Push button
- Green, red, blue and yellow LEDs with individual current-limiting resistors
- Buzzer with a suitable driver circuit
- Raspberry Pi 4
- Breadboard, jumper wires and suitable power supplies

See [hardware wiring](docs/hardware-wiring.md) for the pin map and safety notes.

## Contents

- `arduino/door_activity_checkin.ino`: Arduino firmware, prepared from the working pin map discussed during development; compare it with the exact sketch tested on your hardware before final submission.
- `raspberry-pi/app.py`: MQTT subscriber and Flask dashboard.
- `requirements.txt`: Python dependencies.
- `docs/hardware-wiring.md`: pin map and wiring notes.
- `TESTING.md`: testing plan and recorded development observations.

## Arduino setup

1. Install Arduino IDE and select **Arduino Nano 33 IoT**.
2. Install libraries: WiFiNINA, PubSubClient, Adafruit DHT sensor library, and Adafruit Unified Sensor.
3. Open `arduino/door_activity_checkin.ino`.
4. Replace `YOUR_WIFI_NAME` and `YOUR_WIFI_PASSWORD` locally. Do not commit real credentials.
5. Set `MQTT_SERVER` to the Raspberry Pi's current LAN IP. Check it on the Pi with `hostname -I`.
6. Verify wiring and voltage levels before powering the circuit.
7. Upload the sketch and open Serial Monitor at 115200 baud.

## Raspberry Pi setup

On Raspberry Pi OS:

```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients python3-flask python3-paho-mqtt -y
sudo systemctl enable --now mosquitto
```

The earlier development setup used a Mosquitto listener on all IPv4 interfaces with anonymous access for a trusted local-network test. This is **not secure** for an untrusted network or Internet exposure. Only use it on a private network with the owner's permission; configure authentication and firewall restrictions before broader use.

Example development-only file `/etc/mosquitto/conf.d/door-project.conf`:

```conf
listener 1883 0.0.0.0
allow_anonymous true
```

Restart and verify:

```bash
sudo systemctl restart mosquitto
sudo systemctl status mosquitto
sudo ss -lntp | grep 1883
```

## Run the dashboard

From this folder on the Pi:

```bash
python3 -m pip install --user -r requirements.txt
python3 raspberry-pi/app.py
```

If Raspberry Pi OS blocks user-level pip installation, use the distro packages installed above instead.

Open from a device on the same LAN: `http://<RASPBERRY_PI_IP>:5000`.

## MQTT topics

| Topic | Purpose |
|---|---|
| `door/system/telemetry` | JSON sensor readings and help-request state |
| `door/system/heartbeat` | Arduino online heartbeat every 5 seconds |
| `door/system/alert` | Help request or cancellation event |
| `door/system/status` | Connection status |

The dashboard marks the Arduino offline when the last heartbeat is more than 15 seconds old.

## Privacy and security

- Never commit Wi-Fi passwords, MQTT passwords, API keys or access tokens.
- Keep real credentials only in your local, untracked configuration.
- Do not expose an unauthenticated MQTT broker or Flask development server to the public Internet.
- This prototype dashboard has no login; keep it on a trusted local network.

## Reproducibility note

The source in this folder is a prepared version based on the pin configuration and code discussed during development. Compare it with the exact firmware and Python file tested on your hardware before treating it as the final assessed artefact. Add your own diagrams, photographs, test evidence and demonstration-video link to the final report.
