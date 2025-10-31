<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=Flux%20Queries&fontSize=70&fontColor=fff&animation=twinkling&fontAlignY=40&desc=Grafana%20Dashboard%20Queries%20%7C%20Queries%20para%20Dashboards%20Grafana&descAlignY=65&descSize=18">

## 📊 Flux Queries for Grafana | Queries Flux para o Grafana

<div align="left">

**EN:** This file contains a collection of Flux queries used to build the Grafana dashboards for the Wildfire Detection System. Each query is designed for a specific panel type.
<br>
**PT-BR:** Este arquivo contém uma coleção de queries Flux usadas para construir os dashboards do Grafana para o Sistema de Detecção de Queimadas. Cada query é projetada para um tipo de painel específico.

</div>

### 1. Wildfire Risk by State (Time Series) | Risco de Queimada por Estado

<div align="left">

**EN:** Shows the average risk score over time, grouped by state. Ideal for a `Time series` panel.
<br>
**PT-BR:** Mostra a pontuação média de risco ao longo do tempo, agrupada por estado. Ideal para um painel de `Série Temporal`.

```flux
from(bucket: "fire-detection")
  |> range(start: -6h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["_field"] == "risk_score")
  |> aggregateWindow(every: 5m, fn: mean, createEmpty: false)
  |> group(columns: ["state", "location"])
```

</div>

### 2. Internal vs. External Temperature (Multi-series) | Temperatura Interna vs. Externa

<div align="left">

**EN:** Compares sensor temperature with real weather temperature on the same graph.
<br>
**PT-BR:** Compara a temperatura do sensor com a temperatura real do clima no mesmo gráfico.

```flux
from(bucket: "fire-detection")
  |> range(start: -6h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "sensor_temp" or r["_field"] == "weather_temp")
  |> aggregateWindow(every: 10m, fn: mean, createEmpty: false)
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value")
```

</div>

### 3. Current Status - Gauge (Last Value) | Status Atual - Gauge (Último Valor)

<div align="left">

**EN:** Gets the most recent `risk_score` for a `Gauge` or `Stat` panel.
<br>
**PT-BR:** Obtém o `risk_score` mais recente para um painel `Gauge` ou `Stat`.

```flux
from(bucket: "fire-detection")
  |> range(start: -10m)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["_field"] == "risk_score")
  |> last()
  |> group() // Ungroup to get a single value for the entire system
```

</div>

### 4. Risk Level Distribution (Pie Chart) | Distribuição de Níveis de Risco

<div align="left">

**EN:** Categorizes risk scores from the last 24 hours to show the distribution of "Low", "Medium", "High", and "Critical" levels.
<br>
**PT-BR:** Categoriza as pontuações de risco das últimas 24 horas para mostrar a distribuição dos níveis "Baixo", "Médio", "Alto" e "Crítico".

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["_field"] == "risk_score")
  |> aggregateWindow(every: 1h, fn: last, createEmpty: false)
  |> map(fn: (r) => ({
      r with
      risk_category: if r._value >= 70 then "Critical"
                    else if r._value >= 50 then "High"
                    else if r._value >= 30 then "Medium"
                    else "Low"
    }))
  |> group(columns: ["risk_category"])
  |> count()
```

</div>

### 5. Table of Latest Alerts | Tabela de Últimos Alertas

<div align="left">

**EN:** Lists the last 20 alerts (where `alert` is true) from the past 24 hours, sorted by most recent.
<br>
**PT-BR:** Lista os últimos 20 alertas (onde `alert` é verdadeiro) das últimas 24 horas, ordenados pelos mais recentes.

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["alert"] == "true")
  |> filter(fn: (r) => r["_field"] == "risk_score")
  |> sort(columns: ["_time"], desc: true)
  |> limit(n: 20)
```

</div>

### 6. Heatmap - Risk by Hour and State | Heatmap - Risco por Hora e Estado

<div align="left">

**EN:** Shows the average risk score for each hour over the last 7 days, grouped by state. Ideal for a `Heatmap` panel.
<br>
**PT-BR:** Mostra a pontuação média de risco para cada hora nos últimos 7 dias, agrupada por estado. Ideal para um painel `Heatmap`.

```flux
from(bucket: "fire-detection")
  |> range(start: -7d)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["_field"] == "risk_score")
  |> aggregateWindow(every: 1h, fn: mean, createEmpty: false)
  |> group(columns: ["state"])
```

</div>

### 7. Recommended Thresholds for Panels | Thresholds Recomendados para Painéis

<div align="left">

**EN:**
- **Risk Score (0-100):** Green (0-30), Yellow (30-50), Orange (50-70), Red (70-100).
- **Temperature (°C):** Green (<30), Yellow (30-35), Orange (35-40), Red (>40).
- **Smoke (ppm):** Green (<200), Yellow (200-400), Orange (400-600), Red (>600).
- **Humidity (%):** Red (<30), Orange (30-50), Yellow (50-60), Green (>60).

**PT-BR:**
- **Pontuação de Risco (0-100):** Verde (0-30), Amarelo (30-50), Laranja (50-70), Vermelho (70-100).
- **Temperatura (°C):** Verde (<30), Amarelo (30-35), Laranja (35-40), Vermelho (>40).
- **Fumaça (ppm):** Verde (<200), Amarelo (200-400), Laranja (400-600), Vermelho (>600).
- **Umidade (%):** Vermelho (<30), Laranja (30-50), Amarelo (50-60), Verde (>60).

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>