# Flux query examples

These are examples for the corrected `fire_detection` measurement in bucket `fire-detection`, using InfluxDB 2.x/Flux. They were not executed against a live database during the review.

## Risk score over time by source and state

```flux
from(bucket: "fire-detection")
  |> range(start: -6h)
  |> filter(fn: (r) => r._measurement == "fire_detection" and r._field == "risk_score")
  |> group(columns: ["state", "device"])
  |> aggregateWindow(every: 5m, fn: mean, createEmpty: false)
```

## Latest weather temperature

```flux
from(bucket: "fire-detection")
  |> range(start: -1h)
  |> filter(fn: (r) => r._measurement == "fire_detection" and r._field == "weather_temp")
  |> group(columns: ["state", "device"])
  |> last()
```

## Latest raw metrics

```flux
from(bucket: "fire-detection")
  |> range(start: -1h)
  |> filter(fn: (r) => r._measurement == "fire_detection")
  |> filter(fn: (r) => contains(value: r._field, set: ["sensor_temp", "sensor_humidity", "sensor_smoke", "weather_wind"]))
  |> group(columns: ["state", "device", "_field"])
  |> last()
```

Tag filters require correctly tagged points. The old flat-object writer stored strings as fields, which does not provide the same tag schema. Check the exact node and database versions before relying on these queries.
