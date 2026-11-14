# Testing Plan and Results

Record the test date, setup, expected result, actual result and evidence for each test on the final version.

## Development observations

These observations were reported during project development. Repeat them on the final version and keep screenshots or video as evidence.

| Area | Development observation | Final verification |
|---|---|---|
| DHT22 | Dashboard showed approximately 27.5 °C and 62.8% humidity in one session | Repeat and record |
| PIR | Dashboard showed motion detected | Test with and without movement |
| Ultrasonic | An MQTT distance reading of 13.7 cm appeared; other readings were sometimes missing | Repeat at known distances |
| MQTT | Raspberry Pi received Arduino heartbeat and telemetry messages | Repeat after restart |
| Flask dashboard | Dashboard opened in a browser and displayed live fields | Verify current version |
| Heartbeat | Arduino heartbeat was observed at five-second intervals | Disconnect Arduino and time offline transition |
| Help button | Button event is implemented in the sketch | Press/cancel and capture dashboard response |
| Offline status | Dashboard uses a 15-second heartbeat timeout | Verify OFFLINE after 15 seconds without heartbeat |

## Test procedures

### 1. Sensor readings
1. Power the Arduino and Raspberry Pi.
2. Subscribe to MQTT:
   ```bash
   mosquitto_sub -h localhost -t "door/system/#" -v
   ```
3. Observe temperature, humidity, motion and distance fields.
4. Compare measurements with the environment and known object distances.
5. Record missing or implausible readings as limitations.

### 2. Help request button
1. Open the web dashboard.
2. Press the physical button once.
3. Check the red/green indicators.
4. Confirm an alert appears on `door/system/alert`.
5. Confirm the dashboard displays HELP REQUIRED.
6. Press again to cancel and verify the state returns to No Request.

### 3. Heartbeat and offline detection
1. Confirm heartbeat messages arrive every five seconds.
2. Disconnect Arduino power or stop the sketch.
3. Wait at least 15 seconds.
4. Confirm the dashboard changes to OFFLINE.
5. Restart the Arduino and confirm it returns to ONLINE.

### 4. Wi-Fi and MQTT reconnection
1. Start with both devices on the same Wi-Fi network.
2. Confirm telemetry is received.
3. Briefly disconnect Wi-Fi or stop the broker, with the network owner's permission.
4. Observe Serial Monitor and broker status.
5. Restore the connection and verify that telemetry resumes.

## Evidence to keep

- Arduino Serial Monitor screenshot.
- Raspberry Pi MQTT subscriber output.
- Dashboard screenshots for normal operation and help request.
- Dashboard screenshot showing OFFLINE after heartbeat timeout.
- Short video of sensor and button tests.

Do not mark a test as passed until it has been performed on the final prototype.
