<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=System%20Architecture&fontSize=45&fontColor=fff&animation=twinkling&fontAlignY=40&desc=Detailed%20Project%20Structure&descAlignY=60&descSize=20">

## 🏗️ System Architecture | Arquitetura do Sistema

### 📐 Overview | Visão Geral

<div align="left">

**EN:** The IoT Wildfire Detection and Prevention System follows a **4-layer architecture**:
<br>
**PT-BR:** O Sistema IoT de Detecção e Prevenção de Queimadas segue uma arquitetura em **4 camadas**:

</div>

**EN:**
```
┌─────────────────────────────────────────────────────────────┐
│                      1. SENSOR LAYER                        │
│             (Data Collection - Simulated + Real)            │
└───────────────────────┬─────────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                   2. COMMUNICATION LAYER                    │
│                 (MQTT + HTTP - IoT Protocols)               │
└───────────────────────┬─────────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                     3. PROCESSING LAYER                     │
│          (Node-RED - Analysis + Risk Algorithm)             │
└───────────────────────┬─────────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                       4. OUTPUT LAYER                       │
│          (InfluxDB + Grafana + WhatsApp Alerts)             │
└─────────────────────────────────────────────────────────────┘
```

**PT-BR:**
```
┌─────────────────────────────────────────────────────────────┐
│                    1. CAMADA DE SENSORES                    │
│              (Coleta de dados - Simulado + Real)            │
└───────────────────────┬─────────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                 2. CAMADA DE COMUNICAÇÃO                    │
│              (MQTT + HTTP - Protocolos IoT)                 │
└───────────────────────┬─────────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                3. CAMADA DE PROCESSAMENTO                   │
│       (Node-RED - Análise + Algoritmo de Risco)             │
└───────────────────────┬─────────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                    4. CAMADA DE SAÍDA                       │
│          (InfluxDB + Grafana + WhatsApp Alerts)             │
└─────────────────────────────────────────────────────────────┘
```

### 1️⃣ Sensor Layer (IoT Edge) | Camada de Sensores

<div align="left">

#### Simulated IoT Devices - Wokwi ESP32 | Dispositivos IoT Simulados - Wokwi ESP32
**EN:**
- **Components**: ESP32, DHT22 (Temperature/Humidity), MQ-2 (Smoke), Flame Sensor.
- **Data Collected**: `temperature`, `humidity`, `smoke`, `alert`.
- **Frequency**: Sends data every **10 seconds** via MQTT.

**PT-BR:**
- **Componentes**: ESP32, DHT22 (Temperatura/Umidade), MQ-2 (Fumaça), Sensor de Chama.
- **Dados Coletados**: `temperatura`, `umidade`, `fumaça`, `alerta`.
- **Frequência**: Envio a cada **10 segundos** via MQTT.

#### Real Weather Data - OpenWeather API | Dados Meteorológicos Reais - OpenWeather API
**EN:**
- **Monitored Cities**: Manaus (AM), Campo Grande (MS), Belo Horizonte (MG).
- **Data Collected**: `weather_temp`, `weather_humidity`, `weather_wind`, `weather_conditions`.
- **Frequency**: Automatic collection every **3 minutes**.

**PT-BR:**
- **Cidades Monitoradas**: Manaus (AM), Campo Grande (MS), Belo Horizonte (MG).
- **Dados Coletados**: `temperatura_clima`, `umidade_clima`, `vento_clima`, `condicoes_clima`.
- **Frequência**: Coleta automática a cada **3 minutos**.

</div>

### 2️⃣ Communication Layer | Camada de Comunicação

<div align="left">

#### MQTT (Message Queuing Telemetry Transport)
- **Broker**: `broker.hivemq.com` (Public)
- **Port**: `1883` (non-SSL)
- **Topic**: `fiap/iot/fire-detection/data`
- **QoS**: 0 (Fire and forget)

#### HTTP/REST API
- **Provider**: OpenWeather API
- **Endpoint**: `https://api.openweathermap.org/data/2.5/weather`
- **Method**: `GET`

</div>

### 3️⃣ Processing Layer (Node-RED) | Camada de Processamento

<div align="left">

#### Main Flow | Fluxo Principal

**EN:**
```
┌─────────────┐
│ MQTT Input  │
└──────┬──────┘
       │
       ▼
┌─────────────┐     ┌──────────────┐
│  Save to    ├────→│  Distribute  │
│  Context    │     │  by Device   │
└─────────────┘     └──────┬───────┘
                           │
       ┌───────────────────┴────────────────┐
       │                                    │
       ▼                                    ▼
┌──────────────┐                    ┌──────────────┐
│  Route API   │                    │ Route Wokwi  │
│  (3 cities)  │                    │   (Manual)   │
└──────┬───────┘                    └──────────────┘
       │
       ▼
┌──────────────┐
│ OpenWeather  │
│  API Call    │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ Analyze Fire │
│  Risk Score  │
└──────┬───────┘
       │
       ├────────────────┬─────────────────┐
       │                │                 │
       ▼                ▼                 ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│   InfluxDB   │ │   WhatsApp   │ │   Debug      │
│    Write     │ │  Alert ≥50   │ │   Output     │
└──────────────┘ └──────────────┘ └──────────────┘
```

**PT-BR:**
```
┌─────────────┐
│ Entrada MQTT│
└──────┬──────┘
       │
       ▼
┌─────────────┐     ┌──────────────┐
│ Salvar no   ├────→│  Distribuir  │
│  Contexto   │     │ por Dispositivo│
└─────────────┘     └──────┬───────┘
                           │
       ┌───────────────────┴────────────────┐
       │                                    │
       ▼                                    ▼
┌──────────────┐                    ┌──────────────┐
│ Rota API     │                    │ Rota Wokwi   │
│ (3 cidades)  │                    │   (Manual)   │
└──────┬───────┘                    └──────────────┘
       │
       ▼
┌──────────────┐
│ Chamada API  │
│ OpenWeather  │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ Análise de   │
│ Risco        │
└──────┬───────┘
       │
       ├────────────────┬─────────────────┐
       │                │                 │
       ▼                ▼                 ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│ Escrita      │ │ Alerta Whats │ │ Saída de     │
│ InfluxDB     │ │ (Risco ≥50)  │ │ Debug        │
└──────────────┘ └──────────────┘ └──────────────┘
```

#### Risk Algorithm | Algoritmo de Risco
**EN:** Implemented in a Node-RED `function` node, it calculates a score from 0-100 based on 5 weighted factors: smoke level (40%), internal temperature (25%), external temperature (15%), average humidity (10%), and wind speed (10%).
<br>
**PT-BR:** Implementado em um nó de `função` do Node-RED, calcula uma pontuação de 0 a 100 baseada em 5 fatores com pesos: nível de fumaça (40%), temperatura interna (25%), temperatura externa (15%), umidade média (10%) e velocidade do vento (10%).

</div>

### 4️⃣ Output Layer | Camada de Saída

<div align="left">

#### InfluxDB Cloud - Time Series Database
- **EN:** Stores all sensor and weather data, indexed by `location`, `state`, and `device` tags for efficient querying.
- **PT-BR:** Armazena todos os dados de sensores e clima, indexados pelas tags `location`, `state` e `device` para consultas eficientes.

#### Grafana Cloud - Dashboards
- **EN:** Visualizes real-time and historical data through panels like Gauges, Stats, Time Series graphs, and Tables.
- **PT-BR:** Visualiza dados históricos e em tempo real através de painéis como Medidores (Gauges), Estatísticas, gráficos de Série Temporal e Tabelas.

#### WhatsApp Alerts - Notifications | Alertas WhatsApp - Notificações
- **EN:** Automatically sends a formatted alert message via an API (like CallMeBot) whenever the calculated `risk_score` is 50 or higher.
- **PT-BR:** Envia automaticamente uma mensagem de alerta formatada via uma API (como CallMeBot) sempre que o `risk_score` calculado é 50 ou maior.

</div>

### 🔄 Full Data Flow | Fluxo de Dados Completo

<div align="left">

1. **ESP32** → Reads sensors → Publishes to MQTT topic.
2. **Node-RED** → Subscribes to MQTT topic → Receives data.
3. **Node-RED** → Triggers OpenWeather API call for the corresponding city.
4. **Node-RED** → Combines sensor and weather data → Calculates risk score.
5. **Node-RED** → Formats data into InfluxDB Line Protocol.
6. **InfluxDB** → Stores the time-series data.
7. **Grafana** → Queries InfluxDB → Displays data on dashboards.
8. **WhatsApp** → Sends an alert if the risk score is high (≥ 50).

1. **ESP32** → Lê sensores → Publica no tópico MQTT.
2. **Node-RED** → Se inscreve no tópico MQTT → Recebe os dados.
3. **Node-RED** → Dispara a chamada para a API OpenWeather para a cidade correspondente.
4. **Node-RED** → Combina dados dos sensores e do clima → Calcula a pontuação de risco.
5. **Node-RED** → Formata os dados para o InfluxDB Line Protocol.
6. **InfluxDB** → Armazena os dados de série temporal.
7. **Grafana** → Consulta o InfluxDB → Exibe os dados nos dashboards.
8. **WhatsApp** → Envia um alerta se a pontuação de risco for alta (≥ 50).

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>