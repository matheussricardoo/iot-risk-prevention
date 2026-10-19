# Architecture

The prototype has two independent data-processing paths in flows.json:

1. Sensor/synthetic data stored in Node-RED global context and combined with weather input for an illustrative score.
2. Weather-only data for Manaus, Campo Grande and Belo Horizonte, using separate rules and no smoke sensor.

Both create an intermediate payload with `sensor`, `weather`, `location`, `risk` and ISO timestamp fields. InfluxDB formatters convert this to `[numeric fields, string tags]`, with `time` carrying a Date. The measurement is `fire_detection`.

## Boundaries

MQTT and OpenWeather are input integrations; InfluxDB and notification nodes are external outputs. Those nodes and automatic injectors are disabled by default. The tests execute only pure function code and cannot verify credentials, networking or deployment.

Grafana is a separate consumer of InfluxDB. Its configuration is documented but not provisioned as code here. The repository does not supply a secure Node-RED runtime configuration, TLS broker or cloud infrastructure template.

## Known design limitations

The sensor context is shared, not a per-device time-series store. The synthetic generator intentionally produces varied scenarios. The scoring functions reject missing/non-finite numeric observations; valid zeros are preserved. The sensor route requires actual sensor/simulator context instead of fabricating fallback readings. Wind-unit compatibility requires checking the installed weather node.

The actuator ESP32 listens for alerts but no MQTT output node is present in the flow. The mathematical score and alert thresholds have not been validated as real-world safety predictions. The original academic article is retained as historical source material.
