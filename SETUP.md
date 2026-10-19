# Setup

The tests in the README run without services. A full integration requires Node-RED and the nodes represented by flows.json, an MQTT broker, OpenWeather credentials, and optionally InfluxDB/Grafana and an explicitly configured notification destination. No cloud resource is automatically created by this repository.

## Local Node-RED preparation

With a supported Node.js/npm installation, install dependencies into a project-local directory instead of changing the global system:

```bash
npm install --prefix .local-node-red node-red node-red-contrib-influxdb node-red-node-openweathermap node-red-contrib-whatsapp-cmb
./.local-node-red/node_modules/.bin/node-red --userDir .local-node-red
```

These setup commands were not executed during the portfolio review. Review compatibility of the selected Node-RED and plugin versions. Open the local editor URL printed by Node-RED and import flows.json. Keep automatic/external nodes disabled until configuration is complete.

## Configuration order

1. Inspect the synthetic generator and function outputs using debug nodes, with all external I/O disabled.
2. Configure the desired MQTT broker and a project-specific topic namespace in both the flow and sketches. The sample broker/topic values are shared demonstration settings, not private channels.
3. Configure OpenWeather credentials in the node credential editor. Confirm temperature/wind units from the node version used before interpreting thresholds.
4. Configure an InfluxDB **2.x/Flux-compatible** endpoint, organization, bucket (`fire-detection`) and scoped token. The exported endpoint is a historical example, not an active service guarantee.
5. Keep notification output disabled unless you explicitly choose recipients and authorize sending a test. No recipient or key is supplied by the repository.
6. Enable only the individual nodes needed for your test and inspect Node-RED debug/catch output.

Credentials should stay in local credential storage and must not be exported into Git. `.local-node-red/` is ignored. Cloud services are optional for function-only validation; no claim of free service availability is made.

## Verification

- Run `node --test tests/flows.test.cjs`.
- Inspect a formatted point: payload is `[fields, tags]`; `fields.time` is a Date, state/device are tags.
- Compare sensor and weather-only routes independently.
- Verify a newly written point and its tags in InfluxDB before configuring Grafana panels.
- Verify notifications and devices separately; the MQTT alert output bridge is missing from this export.

The formatting contract follows the [node-red-contrib-influxdb output-node documentation](https://github.com/mblackstock/node-red-contrib-influxdb#output-node). Integration with a running database, broker, weather service, Wokwi or notification provider was not executed during the review.
