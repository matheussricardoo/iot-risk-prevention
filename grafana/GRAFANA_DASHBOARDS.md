<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=Grafana%20Dashboards&fontSize=60&fontColor=fff&animation=twinkling&fontAlignY=40&desc=Configuration%20Guide%20%7C%20Guia%20de%20Configura%C3%A7%C3%A3o&descAlignY=65&descSize=20">

## 📊 Grafana Dashboards Setup | Configuração dos Dashboards Grafana

### 🔗 Connecting to InfluxDB Cloud | Conexão com InfluxDB Cloud

<div align="left">

**EN:**
1. Access your Grafana Cloud instance.
2. Go to **Configuration > Data Sources**.
3. Click **Add data source** and select **InfluxDB**.
4. Configure as follows:
   - **Name:** `InfluxDB - Fire Detection`
   - **Query Language:** `Flux`
   - **URL:** `https://us-east-1-1.aws.cloud2.influxdata.com` (or your region's URL)
   - **Organization:** `your_organization_name`
   - **Token:** Your InfluxDB API Token.
   - **Default Bucket:** `fire-detection`
5. Click **Save & Test**.

**PT-BR:**
1. Acesse sua instância do Grafana Cloud.
2. Vá em **Configuration > Data Sources**.
3. Clique em **Add data source** e selecione **InfluxDB**.
4. Configure da seguinte forma:
   - **Name:** `InfluxDB - Fire Detection`
   - **Query Language:** `Flux`
   - **URL:** `https://us-east-1-1.aws.cloud2.influxdata.com` (ou a URL da sua região)
   - **Organization:** `sua_organizacao`
   - **Token:** Seu Token de API do InfluxDB.
   - **Default Bucket:** `fire-detection`
5. Clique em **Save & Test**.

</div>

## 🌐 DASHBOARD 1: API - Wildfire Detection | Detecção de Queimadas

<div align="left">

- **EN Name:** `🌐 API - Multi-State Wildfire Detection`
- **PT-BR Name:** `🌐 API - Detecção de Queimadas Multi-Estado`
- **Refresh Rate:** `3m`
- **Time Range:** `Last 30 minutes`

</div>

### 📊 Panel 1: Overview - Real-Time Metrics | Visão Geral - Métricas em Tempo Real

<div align="left">

**EN:** Four `Gauge` panels to show the average of the main metrics from the `api` device.
<br>
**PT-BR:** Quatro painéis do tipo `Gauge` para mostrar a média das principais métricas do dispositivo `api`.

- **🌡️ Avg Temperature:** `sensor_temp`
- **💧 Avg Humidity:** `sensor_humidity`
- **💨 Avg Smoke:** `sensor_smoke`
- **🔥 Avg Risk Score:** `risk_score`

**Sample Query (EN/PT-BR):**
```flux
// Replace "sensor_temp" with the desired field for each gauge
from(bucket: "fire-detection")
  |> range(start: -10m)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["device"] == "api")
  |> filter(fn: (r) => r["_field"] == "sensor_temp")
  |> mean()
```

</div>

### 📍 Panel 2: Risk Map by State | Mapa de Risco por Estado

<div align="left">

**EN:** A row with three `Stat` panels, one for each state, showing the latest `risk_score`.
<br>
**PT-BR:** Uma linha com três painéis do tipo `Stat`, um para cada estado, mostrando o último `risk_score` registrado.

- **🌳 Amazonas (AM)**
- **🌾 Mato Grosso do Sul (MS)**
- **⛰️ Minas Gerais (MG)**

**Sample Query (EN/PT-BR):**
```flux
// Replace "AM" with the desired state for each panel
from(bucket: "fire-detection")
  |> range(start: -10m)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["device"] == "api")
  |> filter(fn: (r) => r["state"] == "AM" and r["_field"] == "risk_score")
  |> last()
```

</div>

### 📈 Panel 3: Historical Charts (Last 6 Hours) | Gráficos Históricos

<div align="left">

**EN:** A `Time series` panel showing the historical trend of four key metrics, using overrides to display them on two different Y-axes for clarity.
<br>
**PT-BR:** Um painel de `Série Temporal` mostrando a tendência histórica de quatro métricas chave, usando overrides para exibi-las em dois eixos Y diferentes para maior clareza.

**Combined Query (EN/PT-BR):**
```flux
from(bucket: "fire-detection")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["device"] == "api")
  |> filter(fn: (r) => r["_field"] == "sensor_temp" or r["_field"] == "sensor_humidity" or r["_field"] == "sensor_smoke" or r["_field"] == "risk_score")
  |> aggregateWindow(every: 5m, fn: mean, createEmpty: false)
  |> yield(name: "mean")
```

</div>

### 🚨 Panel 4: Detailed Status Table | Tabela de Status Detalhado

<div align="left">

**EN:** A `Table` panel that pivots the data to show a complete, color-coded overview of the latest metrics for each state.
<br>
**PT-BR:** Um painel de `Tabela` que pivota os dados para mostrar uma visão geral completa e codificada por cores das últimas métricas para cada estado.

**Query (EN/PT-BR):**
```flux
from(bucket: "fire-detection")
  |> range(start: -10m)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["device"] == "api")
  |> filter(fn: (r) => r["_field"] == "sensor_temp" or r["_field"] == "sensor_humidity" or r["_field"] == "sensor_smoke" or r["_field"] == "risk_score" or r["_field"] == "weather_temp" or r["_field"] == "weather_wind")
  |> group(columns: ["state", "location"])
  |> last()
  |> pivot(rowKey: ["state", "location"], columnKey: ["_field"], valueColumn: "_value")
  |> sort(columns: ["risk_score"], desc: true)
```

</div>

## 🤖 DASHBOARD 2: Wokwi - Wildfire Detection | Detecção de Queimadas

<div align="left">

- **EN Name:** `🤖 Wokwi ESP32 - Wildfire Detection`
- **PT-BR Name:** `🤖 Wokwi ESP32 - Detecção de Queimadas`
- **Refresh Rate:** `30s`
- **Time Range:** `Last 30 minutes`

> **📝 Note (EN):** This dashboard has the **SAME structure** as Dashboard 1. The only difference is that all queries must use `filter(fn: (r) => r["device"] == "esp32-wokwi")`.
> <br>
> **📝 Nota (PT-BR):** Este dashboard tem a **MESMA estrutura** do Dashboard 1. A única diferença é que todas as queries devem usar `filter(fn: (r) => r["device"] == "esp32-wokwi")`.

</div>

### 🎨 Recommended Layout | Layout Recomendado

<div align="left">

**EN:**
```
┌──────────────────────────────────────────────────────────┐
│  PANEL 1: REAL-TIME METRICS (4 Gauges)                   │
├──────────────────────────────────────────────────────────┤
│  PANEL 2: RISK MAP BY STATE (3 Stat Panels)              │
├──────────────────────────────────────────────────────────┤
│  PANEL 3: HISTORICAL CHARTS (Time Series)                │
├──────────────────────────────────────────────────────────┤
│  PANEL 4: DETAILED STATUS TABLE (Table)                  │
└──────────────────────────────────────────────────────────┘
```

**PT-BR:**
```
┌──────────────────────────────────────────────────────────┐
│  PAINEL 1: MÉTRICAS EM TEMPO REAL (4 Gauges)             │
├──────────────────────────────────────────────────────────┤
│  PAINEL 2: MAPA DE RISCO POR ESTADO (3 Painéis Stat)     │
├──────────────────────────────────────────────────────────┤
│  PAINEL 3: GRÁFICOS HISTÓRICOS (Série Temporal)          │
├──────────────────────────────────────────────────────────┤
│  PAINEL 4: TABELA DE STATUS DETALHADO (Tabela)           │
└──────────────────────────────────────────────────────────┘
```

</div>

### 🚀 Quick Setup Guide | Guia Rápido de Criação

<div align="left">

**EN:**
1.  **Configure Data Source:** Set up the InfluxDB connection once.
2.  **Create Dashboard 1 (API):**
    - Go to **Dashboards > New Dashboard**.
    - Click **Add new panel** and create the 4 panels using the queries for `device == "api"`.
    - Organize the panels and save the dashboard.
3.  **Create Dashboard 2 (Wokwi):**
    - Instead of starting from scratch, **clone** Dashboard 1.
    - Open the cloned dashboard, go to **Dashboard settings > JSON Model**.
    - Use "Find and Replace" to change all instances of `device" == "api"` to `device" == "esp32-wokwi"`.
    - Save with the new name.

**PT-BR:**
1.  **Configure a Fonte de Dados:** Configure a conexão com o InfluxDB uma única vez.
2.  **Crie o Dashboard 1 (API):**
    - Vá em **Dashboards > New Dashboard**.
    - Clique em **Add new panel** e crie os 4 painéis usando as queries para `device == "api"`.
    - Organize os painéis e salve o dashboard.
3.  **Crie o Dashboard 2 (Wokwi):**
    - Em vez de começar do zero, **clone** o Dashboard 1.
    - Abra o dashboard clonado, vá em **Dashboard settings > JSON Model**.
    - Use "Localizar e Substituir" para trocar todas as instâncias de `device" == "api"` por `device" == "esp32-wokwi"`.
    - Salve com o novo nome.

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>