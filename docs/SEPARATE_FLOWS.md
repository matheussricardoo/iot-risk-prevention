# Separate sensor and weather paths

The sensor path consumes simulated ESP32 data or the synthetic generator. Its illustrative score considers smoke, internal/external temperature, humidity and wind. The weather-only path has no smoke reading and uses its own thresholds. Scores from these paths are not interchangeable measurements.

Device tags distinguish `esp32-wokwi` and `api`; location tags identify cities/states. Verify the source tag in dashboards. The `risk.shouldAlert` field controls whether the flow routes a message to a notification branch; formatting a message is not proof that it was delivered.

Keep external outputs disabled for pure function validation. Enable only a single configured path when diagnosing credentials or field mapping. There is no automated MQTT bridge to the actuator device in this export.
