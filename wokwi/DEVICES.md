# Simulated ESP32 devices

## Sensor device

Use `esp32-sensores.ino`, `diagram-sensores.json` and `libraries-sensores.txt` together. The sketch simulates temperature/humidity and smoke input, and publishes readings on `iot/fire-detection/sensor-data`. Smoke input is an analog simulation, not a calibrated gas measurement.

## Actuator device

Use `esp32-atuadores.ino`, `diagram-atuadores.json` and `libraries-atuadores.txt`. It listens on `iot/fire-detection/alerts` and controls LEDs, buzzer, LCD and the critical-alert output represented in the diagram.

In Wokwi, create two separate ESP32 projects, place each sketch in its own project, copy the matching JSON as diagram.json and add the matching library list. The repository does not validate current Wokwi service limits or pricing.

Both examples use a shared public MQTT broker on plaintext port 1883. Choose a unique topic namespace for an independent experiment and update every participant consistently. There is no secure channel or exclusive topic ownership implied by the examples.

The supplied Node-RED flow has no MQTT output bridge publishing risk alerts to the actuator topic. Test the device independently with a controlled local broker or implement that bridge as a separate future change. The hardware simulation was not executed during the portfolio review.
