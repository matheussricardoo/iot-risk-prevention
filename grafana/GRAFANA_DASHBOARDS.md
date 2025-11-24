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

**For SQL Support (Optional - requires additional setup):**
- To use SQL queries, you need to configure:
  - **Database Settings** → **Database:** Your bucket name (e.g., `fire-detection`)
  - SQL queries must reference tables directly without bucket specification
  - Note: SQL support in InfluxDB Cloud may have limitations

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

**Para Suporte SQL (Opcional - requer configuração adicional):**
- Para usar queries SQL, você precisa configurar:
  - **Database Settings** → **Database:** Nome do seu bucket (ex: `fire-detection`)
  - Queries SQL devem referenciar tabelas diretamente sem especificar bucket
  - Nota: Suporte SQL no InfluxDB Cloud pode ter limitações

</div>

## 🌐 DASHBOARD: API - Multi-State Average Metrics | Métricas Médias Multi-Estado

<div align="left">

- **EN Name:** `🌐 API - Multi-State Average Metrics`
- **PT-BR Name:** `🌐 API - Métricas Médias dos Estados`
- **Refresh Rate:** `3m`
- **Time Range:** `Last 24 hours`
- **Description (EN):** Four gauges showing the average of each metric across all 4 states (AM, MS, MG, and one more state) from API data.
- **Descrição (PT-BR):** Quatro gauges mostrando a média de cada métrica entre todos os 4 estados (AM, MS, MG e mais um estado) dos dados da API.

</div>

### 📊 Four Gauges - Average Metrics Across All States | Quatro Gauges - Métricas Médias de Todos os Estados

<div align="left">

**EN:** Four separate `Gauge` panels placed side-by-side showing the **average** of Temperature, Humidity, Smoke, and Risk Score across ALL states monitored by the API device.
<br>
**PT-BR:** Quatro painéis separados do tipo `Gauge` colocados lado a lado mostrando a **média** de Temperatura, Umidade, Fumaça e Risco entre TODOS os estados monitorados pelo dispositivo API.

**How to create (EN):**
1. Click **Add** → **Visualization**
2. Paste the query below for each metric
3. Select **Gauge** visualization
4. Resize to **3 columns wide**
5. Repeat for all 4 gauges in the same row

**Como criar (PT-BR):**
1. Clique em **Add** → **Visualization**
2. Cole a query abaixo para cada métrica
3. Selecione visualização **Gauge**
4. Redimensione para **3 colunas de largura**
5. Repita para os 4 gauges na mesma linha

---

**Gauge 1 - Average Temperature (Temperatura Média):**
- **Title (EN):** `🌡️ Average Temperature - All States`
- **Title (PT-BR):** `🌡️ Temperatura Média - Todos os Estados`
- **Unit:** `Celsius (°C)`

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "weather_temp")
  |> filter(fn: (r) => r["device"] == "api")
  |> mean()
```

---

**Gauge 2 - Average Humidity (Umidade Média):**
- **Title (EN):** `💧 Average Humidity - All States`
- **Title (PT-BR):** `💧 Umidade Média - Todos os Estados`
- **Unit:** `Percent (0-100)`

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "weather_humidity")
  |> filter(fn: (r) => r["device"] == "api")
  |> mean()
```

---

**Gauge 3 - Average Wind Speed (Velocidade Média do Vento):**
- **Title (EN):** `💨 Average Wind Speed - All States`
- **Title (PT-BR):** `💨 Velocidade Média do Vento - Todos os Estados`
- **Unit:** `velocity (km/h)`

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "weather_wind")
  |> filter(fn: (r) => r["device"] == "api")
  |> mean()
```

---

**Gauge 4 - Average Risk Score (Risco Médio):**
- **Title (EN):** `🔥 Average Risk Score - All States`
- **Title (PT-BR):** `🔥 Risco Médio - Todos os Estados`
- **Unit:** `None`

```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r["_measurement"] == "fire_detection")
  |> filter(fn: (r) => r["_field"] == "risk_score")
  |> filter(fn: (r) => r["device"] == "api")
  |> mean()
```

---

**Thresholds for each gauge:**
- **Temperature:** Green (<30), Yellow (30-35), Orange (35-40), Red (>40)
- **Humidity:** Red (<30), Orange (30-50), Yellow (50-60), Green (>60)
- **Wind Speed:** Green (<20), Yellow (20-30), Orange (30-40), Red (>40)
- **Risk Score:** Green (0-30), Yellow (30-50), Orange (50-70), Red (70-100)

**Additional Configuration:**
- **Calculation:** Last (use the calculated mean value)
- **Orientation:** Horizontal
- **Show threshold labels:** Yes
- **Show threshold markers:** Yes

</div>

### 🎨 Dashboard Layout | Layout do Dashboard

### 🎨 Dashboard Layout | Layout do Dashboard

<div align="left">

**EN:**
```
┌────────────────────────────────────────────────────────────────┐
│                    🌐 API - MULTI-STATE METRICS                │
├────────────────────────────────────────────────────────────────┤
│  🌡️ Temp    │  💧 Humidity  │  💨 Wind     │  🔥 Risk Score  │
│  Average    │  Average      │  Speed Avg   │  Average        │
│  (Gauge)    │  (Gauge)      │  (Gauge)     │  (Gauge)        │
└────────────────────────────────────────────────────────────────┘
```

**PT-BR:**
```
┌────────────────────────────────────────────────────────────────┐
│              🌐 API - MÉTRICAS DOS ESTADOS                     │
├────────────────────────────────────────────────────────────────┤
│  🌡️ Temp    │  💧 Umidade   │  💨 Vento    │  🔥 Risco       │
│  Média      │  Média        │  Vel. Média  │  Médio          │
│  (Gauge)    │  (Gauge)      │  (Gauge)     │  (Gauge)        │
└────────────────────────────────────────────────────────────────┘
```

</div>

### 🚀 Quick Setup Guide | Guia Rápido de Criação

<div align="left">

**EN:**
1. **Configure Data Source:** 
   - Go to **Configuration > Data Sources**
   - Add **InfluxDB** with Flux query language
   - Test connection

2. **Create Dashboard:**
   - Go to **Dashboards > New Dashboard**
   - Click **Add visualization**

3. **Add Gauge 1 (Temperature):**
   - Paste the Temperature query
   - Select **Gauge** visualization
   - Set title: "🌡️ Average Temperature - All States"
   - Set unit: Celsius (°C)
   - Configure thresholds (Green <30, Yellow 30-35, Orange 35-40, Red >40)
   - Resize to **3 columns wide**
   - Click **Apply**

4. **Add Gauges 2, 3, 4:**
   - Repeat step 3 for Humidity, Wind Speed, and Risk Score
   - Place all 4 gauges in the same row

5. **Save Dashboard:**
   - Click **Save dashboard** (disk icon)
   - Name: "🌐 API - Multi-State Average Metrics"
   - Click **Save**

**PT-BR:**
1. **Configure a Fonte de Dados:**
   - Vá em **Configuration > Data Sources**
   - Adicione **InfluxDB** com linguagem de query Flux
   - Teste a conexão

2. **Crie o Dashboard:**
   - Vá em **Dashboards > New Dashboard**
   - Clique em **Add visualization**

3. **Adicione o Gauge 1 (Temperatura):**
   - Cole a query de Temperatura
   - Selecione visualização **Gauge**
   - Defina título: "🌡️ Temperatura Média - Todos os Estados"
   - Defina unidade: Celsius (°C)
   - Configure limites (Verde <30, Amarelo 30-35, Laranja 35-40, Vermelho >40)
   - Redimensione para **3 colunas de largura**
   - Clique em **Apply**

4. **Adicione os Gauges 2, 3, 4:**
   - Repita o passo 3 para Umidade, Velocidade do Vento e Risco
   - Coloque todos os 4 gauges na mesma linha

5. **Salve o Dashboard:**
   - Clique em **Save dashboard** (ícone de disco)
   - Nome: "🌐 API - Métricas Médias dos Estados"
   - Clique em **Save**

</div>

### 📝 Important Notes | Notas Importantes

<div align="left">

**EN:**
- These queries calculate the **average (mean)** of all readings from the last 24 hours across ALL states
- The `device == "api"` filter ensures only API data is included (not ESP32/Wokwi data)
- If you have data from 4 states (AM, MS, MG, and one more), the average will include all of them automatically
- Refresh rate of 3 minutes ensures data is up-to-date without overloading the system

**PT-BR:**
- Estas queries calculam a **média** de todas as leituras das últimas 24 horas entre TODOS os estados
- O filtro `device == "api"` garante que apenas dados da API sejam incluídos (não dados do ESP32/Wokwi)
- Se você tiver dados de 4 estados (AM, MS, MG e mais um), a média incluirá todos automaticamente
- Taxa de atualização de 3 minutos garante dados atualizados sem sobrecarregar o sistema

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>