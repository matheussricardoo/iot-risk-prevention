# Grafana dashboards

Use an independently configured Grafana instance and an InfluxDB 2.x/Flux data source. Supply its URL, organization, bucket and a read-scoped token locally. No dashboard or data source is automatically provisioned.

Validate ingestion first, then use [QUERIES.md](QUERIES.md) for:

- Risk score time series, grouped by state/source tags.
- Last observed temperature, humidity, smoke and wind.
- Separate views for synthetic/sensor and weather-only sources.

Display data age and source alongside scores. A historical or synthetic score must not be shown as a current field observation. No online dashboard or alert SLA is confirmed.

The corrected writer stores `state`, `device`, `location`, `region`, `risk_level` and `alert` as tags. Old data written with these as fields may need separate queries or a fresh measurement; changing the writer does not migrate existing data.
