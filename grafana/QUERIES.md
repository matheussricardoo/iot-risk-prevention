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
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "risk_score")
  |> aggregateWindow(every: 5m, fn: mean, createEmpty: false)
```

</div>

### 2. Real Weather Data - Temperature (Time Series) | Dados Meteorológicos Reais - Temperatura

<div align="left">

**EN:** Shows real weather temperature from OpenWeather API over time by state.
<br>
**PT-BR:** Mostra a temperatura meteorológica real da API OpenWeather ao longo do tempo por estado.

```flux
from(bucket: "fire-detection")
  |> range(start: -6h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "weather_temp")
  |> aggregateWindow(every: 10m, fn: mean, createEmpty: false)
```

</div>

### 3. Current Status - Gauge (Last Value) | Status Atual - Gauge (Último Valor)

<div align="left">

**EN:** Gets the most recent value for a `Gauge` or `Stat` panel. Replace `risk_score` with the desired field: `weather_temp`, `weather_humidity`, `weather_wind`, or `risk_score`.
<br>
**PT-BR:** Obtém o valor mais recente para um painel `Gauge` ou `Stat`. Substitua `risk_score` pelo campo desejado: `weather_temp`, `weather_humidity`, `weather_wind` ou `risk_score`.

```flux
from(bucket: "fire-detection")
  |> range(start: -1h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "risk_score")  // Change field here
  |> last()
  |> mean()
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

### 7. Real Weather Metrics by State (Time Series) | Métricas Meteorológicas Reais por Estado

<div align="left">

**EN:** Shows real weather data (temperature, humidity, wind) over time for each state. Ideal for a `Time series` panel with multiple Y-axes.
<br>
**PT-BR:** Mostra dados meteorológicos reais (temperatura, umidade, vento) ao longo do tempo para cada estado. Ideal para um painel de `Série Temporal` com múltiplos eixos Y.

```flux
from(bucket: "fire-detection")
  |> range(start: -6h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "weather_temp" or r["_field"] == "weather_humidity" or r["_field"] == "weather_wind" or r["_field"] == "state")
  |> aggregateWindow(every: 5m, fn: last, createEmpty: false)
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value")
  |> group(columns: ["state"])
```

**Configuration Tips (EN):**
- Use **Field overrides** to assign different Y-axes: Temperature (left), Humidity (right), Wind (right-2)
- Enable **Legend** with format: `{{state}} - {{_field}}`
- Set different colors per metric type

**Dicas de Configuração (PT-BR):**
- Use **Field overrides** para atribuir diferentes eixos Y: Temperatura (esquerda), Umidade (direita), Vento (direita-2)
- Ative a **Legenda** com formato: `{{state}} - {{_field}}`
- Defina cores diferentes por tipo de métrica

</div>

### 8. Risk Heatmap Over Time by State | Heatmap de Risco ao Longo do Tempo por Estado

<div align="left">

**EN:** Visualizes risk intensity over time for each state. Ideal for a `Heatmap` panel to identify patterns.
<br>
**PT-BR:** Visualiza a intensidade do risco ao longo do tempo para cada estado. Ideal para um painel `Heatmap` para identificar padrões.

```flux
from(bucket: "fire-detection")
  |> range(start: -6h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "risk_score" or r["_field"] == "state")
  |> aggregateWindow(every: 10m, fn: last, createEmpty: false)
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value")
  |> group(columns: ["state"])
  |> keep(columns: ["_time", "state", "risk_score"])
```

**Configuration Tips (EN):**
- **Color scheme:** Green → Yellow → Orange → Red
- **Cell gap:** 2px
- **Y-axis:** Display state names
- **X-axis:** Time buckets

**Dicas de Configuração (PT-BR):**
- **Esquema de cores:** Verde → Amarelo → Laranja → Vermelho
- **Espaçamento de célula:** 2px
- **Eixo Y:** Mostrar nomes dos estados
- **Eixo X:** Blocos de tempo

</div>

### 9. Critical Alerts Table (High/Critical Only) | Tabela de Alertas Críticos (Apenas Alto/Crítico)

<div align="left">

**EN:** Shows only HIGH and CRITICAL risk events from the last 24 hours with real weather data. Ideal for a `Table` panel focused on actionable alerts.
<br>
**PT-BR:** Mostra apenas eventos de risco ALTO e CRÍTICO das últimas 24 horas com dados meteorológicos reais. Ideal para um painel de `Tabela` focado em alertas acionáveis.

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> last()
  |> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
  |> filter(fn: (r) => exists r.risk_score and float(v: r.risk_score) >= 50.0)
  |> sort(columns: ["_time"], desc: true)
  |> limit(n: 50)
  |> keep(columns: ["_time", "state", "location", "risk_score", "weather_temp", "weather_humidity", "weather_wind"])
```

**Configuration Tips (EN):**
- Add **Cell color** override for `risk_score` column: 50-70 (orange), 70-100 (red)
- Hide unnecessary columns (`_measurement`, `_start`, `_stop`)
- Rename columns: `_time` → "Timestamp", `state` → "State", `risk_score` → "Risk", `weather_temp` → "Temperature", `weather_humidity` → "Humidity", `weather_wind` → "Wind"

**Dicas de Configuração (PT-BR):**
- Adicione override de **Cor de célula** para coluna `risk_score`: 50-70 (laranja), 70-100 (vermelho)
- Oculte colunas desnecessárias (`_measurement`, `_start`, `_stop`)
- Renomeie colunas: `_time` → "Timestamp", `state` → "Estado", `risk_score` → "Risco", `weather_temp` → "Temperatura", `weather_humidity` → "Umidade", `weather_wind` → "Vento"

</div>

### 10. Overall System Risk Gauge | Gauge de Risco Geral do Sistema

<div align="left">

**EN:** Displays the maximum risk score across all states in the last hour. Ideal for a `Gauge` panel as a system-wide health indicator.
<br>
**PT-BR:** Exibe a pontuação máxima de risco entre todos os estados na última hora. Ideal para um painel `Gauge` como indicador de saúde geral do sistema.

```flux
from(bucket: "fire-detection")
  |> range(start: -1h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "risk_score")
  |> max()
```

**Configuration Tips (EN):**
- **Thresholds:** Green (0-30), Yellow (30-50), Orange (50-70), Red (70-100)
- **Show threshold markers** on gauge
- **Title:** "🎯 Maximum System Risk"
- **Unit:** None (dimensionless)

**Dicas de Configuração (PT-BR):**
- **Limites:** Verde (0-30), Amarelo (30-50), Laranja (50-70), Vermelho (70-100)
- **Mostrar marcadores de limite** no medidor
- **Título:** "🎯 Risco Máximo do Sistema"
- **Unidade:** Nenhuma (adimensional)

</div>

### 11. Risk Level Distribution (Pie Chart) | Distribuição de Níveis de Risco (Gráfico Pizza)

<div align="left">

**EN:** Shows the proportion of time spent in each risk category over the last 24 hours. Ideal for a `Pie chart` panel for trend analysis.
<br>
**PT-BR:** Mostra a proporção de tempo gasto em cada categoria de risco nas últimas 24 horas. Ideal para um painel de `Gráfico de Pizza` para análise de tendências.

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection" and r["_field"] == "risk_score")
  |> aggregateWindow(every: 10m, fn: last, createEmpty: false)
  |> map(fn: (r) => ({
      r with
      risk_level: if r._value >= 70 then "CRÍTICO"
                  else if r._value >= 50 then "ALTO"
                  else if r._value >= 30 then "MÉDIO"
                  else "BAIXO"
    }))
  |> group(columns: ["risk_level"])
  |> count()
  |> group()
```

**Configuration Tips (EN):**
- **Display labels:** Percent + Name
- **Colors:** BAIXO (green), MÉDIO (yellow), ALTO (orange), CRÍTICO (red)
- **Legend:** Bottom, show values
- **Title:** "📊 Risk Level Distribution (24h)"

**Dicas de Configuração (PT-BR):**
- **Rótulos de exibição:** Porcentagem + Nome
- **Cores:** BAIXO (verde), MÉDIO (amarelo), ALTO (laranja), CRÍTICO (vermelho)
- **Legenda:** Embaixo, mostrar valores
- **Título:** "📊 Distribuição de Níveis de Risco (24h)"

</div>

### 12. Risk Score by State (Bar Gauge) | Pontuação de Risco por Estado (Barra)

<div align="left">

**EN:** Shows the current risk score for each state side-by-side. Ideal for a `Bar gauge` panel for quick state comparison.
<br>
**PT-BR:** Mostra a pontuação de risco atual para cada estado lado a lado. Ideal para um painel de `Barra` para comparação rápida entre estados.

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "risk_score" or r["_field"] == "state")
  |> last()
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value")
  |> group(columns: ["state"])
  |> keep(columns: ["state", "risk_score"])
```

**Configuration Tips (EN):**
- **Orientation:** Horizontal
- **Display mode:** Gradient
- **Show values:** On bar
- **Thresholds:** Same as gauge (Green/Yellow/Orange/Red)

**Dicas de Configuração (PT-BR):**
- **Orientação:** Horizontal
- **Modo de exibição:** Gradiente
- **Mostrar valores:** Na barra
- **Limites:** Mesmos do gauge (Verde/Amarelo/Laranja/Vermelho)

</div>

### 13. Recommended Thresholds for Panels | Thresholds Recomendados para Painéis

<div align="left">

**EN:**
- **Risk Score (0-100):** Green (0-30), Yellow (30-50), Orange (50-70), Red (70-100).
- **Weather Temperature (°C):** Green (<30), Yellow (30-35), Orange (35-40), Red (>40).
- **Weather Humidity (%):** Red (<30), Orange (30-50), Yellow (50-60), Green (>60).
- **Wind Speed (km/h):** Green (<20), Yellow (20-30), Orange (30-40), Red (>40).

**PT-BR:**
- **Pontuação de Risco (0-100):** Verde (0-30), Amarelo (30-50), Laranja (50-70), Vermelho (70-100).
- **Temperatura Meteorológica (°C):** Verde (<30), Amarelo (30-35), Laranja (35-40), Vermelho (>40).
- **Umidade Meteorológica (%):** Vermelho (<30), Laranja (30-50), Amarelo (50-60), Verde (>60).
- **Velocidade do Vento (km/h):** Verde (<20), Amarelo (20-30), Laranja (30-40), Vermelho (>40).

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>