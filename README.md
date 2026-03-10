<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=IoT%20Fire%20Prevention&fontSize=50&fontColor=fff&animation=twinkling&fontAlignY=40&desc=Node-RED%20|%20InfluxDB%20|%20Grafana%20|%20ESP32&descAlignY=60&descSize=18">

<p align="center">
  <i>An intelligent IoT system for wildfire risk monitoring and prevention, collecting data from simulated sensors and real weather conditions across multiple Brazilian states.</i>
</p>

---

### Project Goal

<div align="left">

Develop an early warning system for wildfire prevention, monitoring in real-time:
- Temperature (sensor + real weather)
- Humidity (sensor + real weather)
- Smoke Level (IoT sensor)
- Wind Speed (real weather)
- Weather Conditions (OpenWeather API)

</div>

### Project Demo

<div align="center">

[![Watch the video](https://img.youtube.com/vi/ucTAAcTbRl0/maxresdefault.jpg)](https://youtu.be/ucTAAcTbRl0)

<p align="center">
  <i>Watch the full project presentation and demonstration on YouTube</i>
</p>

</div>

### Monitored States

<div align="center">

| State | City |
|:---:|:---:|
| Amazonas (AM) | Manaus |
| Mato Grosso do Sul (MS) | Campo Grande |
| Minas Gerais (MG) | Belo Horizonte |

</div>

### System Architecture

<div align="left">

4-layer architecture:
1. Sensor Layer - Data collection (Wokwi ESP32 + OpenWeather API)
2. Communication Layer - MQTT + HTTP protocols
3. Processing Layer - Node-RED flow orchestration
4. Output Layer - InfluxDB + Grafana + WhatsApp alerts

</div>

<div align="left">

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
InfluxDB Configuration:
```
URL: https://us-east-1-1.aws.cloud2.influxdata.com
Org: your_organization
Bucket: fire-detection
Measurement: fire_detection
```
</div>

### Fire Risk Algorithm

<div align="left">

The system calculates a risk score (0-100) based on 5 factors:

</div>

<div align="center">

| Factor | Weight | Critical | High | Medium | Low |
|:---:|:---:|:---:|:---:|:---:|:---:|
| Smoke Level | 40% | >600 ppm (+40) | 400-600 (+30) | 200-400 (+15) | <200 (+0) |
| Internal Temp | 25% | >50°C (+25) | 40-50°C (+20) | 35-40°C (+10) | <35°C (+0) |
| External Temp | 15% | >35°C (+15) | 30-35°C (+10) | 25-30°C (+5) | <25°C (+0) |
| Avg Humidity | 10% | <30% (+10) | - | 30-50% (+5) | >50% (+0) |
| Wind Speed | 10% | >40 km/h (+10) | - | 20-40 (+5) | <20 (+0) |

</div>

### Risk Classification

<div align="center">

| Score | Level | Action |
|:---:|:---:|:---|
| 70-100 | CRITICAL | Evacuation + Fire Dept (193) |
| 50-69 | HIGH | Maximum attention + Authorities |
| 30-49 | MEDIUM | Intensive monitoring |
| 0-29 | LOW | Continuous monitoring |

</div>

<p align="center">
  <i>WhatsApp alerts are automatically sent when score ≥ 50</i>
</p>

### Technologies

<div align="center">
  <img src="https://img.shields.io/badge/Node.js-339933?style=for-the-badge&logo=nodedotjs&logoColor=white" alt="Node.js"/>
  <img src="https://img.shields.io/badge/MQTT-660066?style=for-the-badge&logo=mqtt&logoColor=white" alt="MQTT"/>
  <img src="https://img.shields.io/badge/InfluxDB-22ADF6?style=for-the-badge&logo=influxdb&logoColor=white" alt="InfluxDB"/>
  <img src="https://img.shields.io/badge/Grafana-F46800?style=for-the-badge&logo=grafana&logoColor=white" alt="Grafana"/>
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino"/>
</div>

<div align="center">

| Category | Technology | Description |
|:---:|:---:|:---|
| Hardware/Simulation | Wokwi, ESP32, DHT22, MQ-2 | Online ESP32 simulator with sensors |
| Backend | Node-RED, MQTT, OpenWeather API | IoT flow orchestration |
| Database | InfluxDB Cloud | Time-series database |
| Visualization | Grafana Cloud | Dashboards and analytics |
| Notifications | WhatsApp API (CallMeBot/Twilio) | Real-time alerts |

</div>

### Project Structure

```
iot-risk-prevention/
├── README.md                 # This file
├── SETUP.md                  # Setup guide
├── flows.json                # Node-RED flow
├── wokwi/
│   ├── esp32-code.ino          # Arduino code
│   ├── diagram.json            # Circuit diagram
│   └── libraries.txt           # Required libraries
├── grafana/
│   ├── GRAFANA_DASHBOARDS.md   # Dashboard guide
│   └── QUERIES.md              # Flux queries
├── docs/
│   └── ARCHITECTURE.md         # System architecture
└── .gitignore
```

### Quick Start

<div align="left">

```bash
# 1. Clone the repository
git clone https://github.com/matheussricardoo/iot-risk-prevention.git

# 2. Install Node-RED
npm install -g --unsafe-perm node-red

# 3. Install dependencies
cd ~/.node-red
npm install node-red-node-openweathermap node-red-contrib-influxdb node-red-contrib-whatsapp-cmb

# 4. Start Node-RED
node-red

# 5. Import flows.json (http://localhost:1880)
# Menu → Import → Clipboard → Paste flows.json content → Deploy
```

</div>

<p align="center">
  <b>Complete guide available at:</b><br>
  <a href="./SETUP.md">SETUP.md</a> | <a href="./docs/ARCHITECTURE.md">ARCHITECTURE.md</a>
</p>

### Testing the System

<div align="center">

| Test | Temperature | Humidity | Smoke | Expected |
|:---:|:---:|:---:|:---:|:---:|
| Normal Conditions | 28°C | 65% | 120 ppm | LOW RISK |
| High Risk | 42°C | 35% | 450 ppm | HIGH RISK + WhatsApp Alert |
| Critical Fire | 55°C | 20% | 750 ppm | CRITICAL + Urgent Alert |

</div>

### Grafana Dashboard Panels

<div align="center">

| Panel | Type | Description |
|:---:|:---:|:---|
| 1 | Gauges | Current metrics (temp, humidity, smoke, risk) |
| 2 | Stats | Risk score by state (AM, MS, MG) |
| 3 | Time Series | Historical metrics (last 6 hours) |
| 4 | Table | Latest records (all states) |

</div>

### WhatsApp Alert Example

<div align="center">

```
FIRE RISK ALERT: CRITICAL

RISK SCORE: 85/100

──────────────────
LOCATION
City: Manaus
State: Amazonas (AM)

──────────────────
IoT SENSOR DATA
Temperature: 55.0°C
Humidity: 20.0%
Smoke: 750 ppm

──────────────────
REAL WEATHER CONDITIONS
Temperature: 38.5°C
Humidity: 25.0%
Wind: 45.2 km/h

──────────────────
2025-10-31 16:42:09

IMMEDIATE ACTION REQUIRED!
Fire Department: 193
```

</div>

### Security Best Practices

<div align="center">

| Practice | Description |
|:---:|:---|
| ✅ | Use environment variables for tokens/API keys |
| ✅ | Never commit credentials to Git |
| ✅ | Use HTTPS for all external connections |
| ✅ | Implement rate limiting on APIs |
| ✅ | Configure data retention in InfluxDB |
| ✅ | Enable authentication in Node-RED |
| ✅ | Use TLS/SSL for MQTT broker (production) |

</div>

### Future Improvements

<div align="center">

| Priority | Feature |
|:---:|:---|
| Short Term | Add more Brazilian states |
| Short Term | Telegram integration |
| Medium Term | Machine Learning for risk prediction |
| Medium Term | NASA FIRMS satellite integration |
| Long Term | Real sensor network (not simulated) |
| Long Term | Civil Defense integration |

</div>

### Contributing

<div align="left">

This is an academic project. Contributions are welcome!

</div>

```bash
# Fork the project
# Create a branch
git checkout -b feature/new-feature

# Commit your changes
git commit -m 'Add new feature'

# Push to the branch
git push origin feature/new-feature

# Open a Pull Request
```

### License

<p align="center">
  <a href="LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License: MIT">
  </a>
  <br>
  <i>This project is licensed under the MIT License. See the <a href="LICENSE">LICENSE</a> file for details.</i>
</p>

### Acknowledgments

<div align="center">

| Platform | Thanks for |
|:---:|:---|
| Wokwi | Excellent simulation platform |
| InfluxData | Free InfluxDB Cloud |
| Grafana Labs | Free Grafana Cloud |
| OpenWeather | Weather API |
| HiveMQ | Public MQTT broker |

</div>

---

<p align="center">
  <b>WARNING</b><br>
  <i>This is an educational monitoring system. In real emergencies, always call the fire department (193) and follow official safety protocols.</i>
</p>

<p align="center">
  <b>Wildfire prevention saves lives and preserves the environment!</b>
</p>

### Authors

<div align="center">
<table align="center">
  <tr>
    <td align="center">
      <a href="https://github.com/matheussricardoo" target="_blank">
        <b>Matheus Ricardo</b>
      </a>
      <br>
      <a href="https://github.com/matheussricardoo" target="_blank">
        <img src="https://skillicons.dev/icons?i=github" alt="GitHub"/>
      </a>
      <a href="https://www.linkedin.com/in/matheus-ricardo-426452266/" target="_blank">
        <img src="https://skillicons.dev/icons?i=linkedin" alt="LinkedIn"/>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/PriHerculano" target="_blank">
        <b>Priscila Herculano</b>
      </a>
      <br>
      <a href="https://github.com/PriHerculano" target="_blank">
        <img src="https://skillicons.dev/icons?i=github" alt="GitHub"/>
      </a>
      <a href="https://www.linkedin.com/in/priscilaherculano/" target="_blank">
        <img src="https://skillicons.dev/icons?i=linkedin" alt="LinkedIn"/>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/bpdc" target="_blank">
        <b>Beatriz Pimenta</b>
      </a>
      <br>
      <a href="https://github.com/bpdc" target="_blank">
        <img src="https://skillicons.dev/icons?i=github" alt="GitHub"/>
      </a>
      <a href="https://www.linkedin.com/in/beatriz-pimenta-de-camargo/" target="_blank">
        <img src="https://skillicons.dev/icons?i=linkedin" alt="LinkedIn"/>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/Nicolai012" target="_blank">
        <b>Nicolai JZ</b>
      </a>
      <br>
      <a href="https://github.com/Nicolai012" target="_blank">
        <img src="https://skillicons.dev/icons?i=github" alt="GitHub"/>
      </a>
      <a href="https://www.linkedin.com/in/nicolai-jz/" target="_blank">
        <img src="https://skillicons.dev/icons?i=linkedin" alt="LinkedIn"/>
      </a>
    </td>
  </tr>
</table>
</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>