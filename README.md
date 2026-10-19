# IoT Risk Prevention

Academic environmental-monitoring prototype combining simulated ESP32 readings, Node-RED processing, OpenWeather data and InfluxDB/Grafana examples. It is an educational risk-score demonstration, not a validated wildfire prediction or emergency system.

## Implemented artifacts

- Two ESP32 sketches and matching Wokwi diagrams for sensors and actuators.
- Node-RED flow with a sensor/simulator path and a separate weather-only path.
- Rule-based risk scoring, InfluxDB point formatting and WhatsApp message formatting.
- Example Flux queries and dashboard setup notes.

The exported flow does **not** contain an MQTT output node connecting calculated alerts back to the actuator sketch. The device sketch and alert input topic exist, but that end-to-end actuator integration is not implemented in the exported flow.

## Local validation without external services

Requires Node.js 18+ for the built-in test runner; no packages or credentials are needed for these checks:

```bash
node --test tests/flows.test.cjs
```

Tests check function syntax, fields/tags serialization, timestamps, invalid measurements and opt-in external nodes. They do not start Node-RED, connect to a broker, write to InfluxDB or send messages.

## Setup and architecture

See [SETUP.md](SETUP.md), [architecture](docs/ARCHITECTURE.md), [separate paths](docs/SEPARATE_FLOWS.md), [devices](wokwi/DEVICES.md) and [Grafana queries](grafana/QUERIES.md).

```text
ESP32 sensor or synthetic generator -> Node-RED -> rule-based risk -> InfluxDB
OpenWeather cities ---------------> Node-RED -> weather-only score -> InfluxDB
                                                        -> WhatsApp formatter/output
InfluxDB -> Grafana
ESP32 actuator <- MQTT alerts (bridge from this flow is not implemented)
```

Automatic injectors and external I/O nodes are disabled in the exported flow. Review credentials, recipients and possible charges before explicitly enabling them. Importing a flow does not establish that any cloud service is active.

## Limitations

- Public MQTT examples use plaintext port 1883; no TLS/authentication guarantee.
- Sensor/synthetic values are not field measurements. Weather values depend on an external API and its units.
- Risk thresholds are illustrative rules, not an independently validated prediction model.
- No demonstrated end-to-end deployment, production monitoring or alert delivery guarantee.
- External services may require credentials, quota management and paid plans. No free-tier assumption is made.

## Team and license

Original credits preserved: [Matheus Ricardo](https://github.com/matheussricardoo), [Priscila Herculano](https://github.com/PriHerculano), [Beatriz Pimenta](https://github.com/bpdc), and [Nicolai JZ](https://github.com/Nicolai012).

[Matheus on LinkedIn](https://www.linkedin.com/in/matheussricardoo/). Original project demonstration: [video](https://youtu.be/ucTAAcTbRl0); its current availability was not verified. See [LICENSE](LICENSE). The original Portuguese article remains in `article/` with its existing authorship.
