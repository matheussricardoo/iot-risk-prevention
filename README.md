<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=IoT%20Fire%20Prevention&fontSize=50&fontColor=fff&animation=twinkling&fontAlignY=40&desc=Node-RED%20|%20InfluxDB%20|%20Grafana%20|%20ESP32&descAlignY=60&descSize=18">

<p align="center">
  <i>🔥 An intelligent IoT system for wildfire risk monitoring and prevention, collecting data from simulated sensors and real weather conditions across multiple Brazilian states.</i>
</p>

<p align="center">
  <i>🔥 Um sistema IoT inteligente de monitoramento e prevenção de risco de queimadas, coletando dados de sensores simulados e condições meteorológicas reais de múltiplos estados brasileiros.</i>
</p>

---

### 🎯 Project Goal | Objetivo do Projeto

<div align="left">

**EN:** Develop an early warning system for wildfire prevention, monitoring in real-time:
- **Temperature** (sensor + real weather)
- **Humidity** (sensor + real weather)
- **Smoke Level** (IoT sensor)
- **Wind Speed** (real weather)
- **Weather Conditions** (OpenWeather API)

**PT-BR:** Desenvolver um sistema de alerta precoce para prevenção de queimadas, monitorando em tempo real:
- **Temperatura** (sensor + clima real)
- **Umidade** (sensor + clima real)
- **Nível de Fumaça** (sensor IoT)
- **Velocidade do Vento** (clima real)
- **Condições Meteorológicas** (API OpenWeather)

</div>

### 🗺️ Monitored States | Estados Monitorados

<div align="center">

| State | City | Emoji |
|:---:|:---:|:---:|
| Amazonas (AM) | Manaus | 🌳 |
| Mato Grosso do Sul (MS) | Campo Grande | 🌾 |
| Minas Gerais (MG) | Belo Horizonte | ⛰️ |

</div>

### 🏗️ System Architecture | Arquitetura do Sistema

<div align="left">

**EN:** 4-layer architecture:
1. **Sensor Layer** - Data collection (Wokwi ESP32 + OpenWeather API)
2. **Communication Layer** - MQTT + HTTP protocols
3. **Processing Layer** - Node-RED flow orchestration
4. **Output Layer** - InfluxDB + Grafana + WhatsApp alerts

**PT-BR:** Arquitetura em 4 camadas:
1. **Camada de Sensores** - Coleta de dados (Wokwi ESP32 + API OpenWeather)
2. **Camada de Comunicação** - Protocolos MQTT + HTTP
3. **Camada de Processamento** - Orquestração de fluxos Node-RED
4. **Camada de Saída** - InfluxDB + Grafana + Alertas WhatsApp

</div>

<div align="left">

**EN:**
```
┌──────────────────────────────────────────────────────────┐
│                       SENSOR LAYER                       │
├──────────────────────────────────────────────────────────┤
│ Wokwi ESP32 (Simulated)        OpenWeather API (Real)      │
│  • Temperature                 • Temperature               │
│  • Humidity                    • Humidity                  │
│  • Smoke Sensor                • Wind Speed                │
│  • Flame Sensor                • Weather Conditions        │
└───────────────┬──────────────────────┬─────────────────────┘
                │                      │
                │ MQTT (HiveMQ)        │ HTTP/REST
                │                      │
┌───────────────▼──────────────────────▼─────────────────────┐
│                      PROCESSING LAYER                      │
├──────────────────────────────────────────────────────────┤
│                        Node-RED                            │
│  • Receives MQTT data from ESP32                           │
│  • Queries real-time weather (3 states)                    │
│  • Risk analysis algorithm                                 │
│  • Data formatting for InfluxDB                            │
│  • WhatsApp alert generation                               │
└───────────────┬──────────────────────┬─────────────────────┘
                │                      │
                │ InfluxDB Line        │ WhatsApp API
                │ Protocol             │
┌───────────────▼──────────────────────▼─────────────────────┐
│                       OUTPUT LAYER                         │
├──────────────────────────────────────────────────────────┤
│  InfluxDB Cloud      Grafana Cloud      WhatsApp           │
│  • Storage           • Dashboards       • Alerts           │
│  • Time series       • Visualization    • Critical         │
│  • Analytics         • Analysis         • High             │
└──────────────────────────────────────────────────────────┘
```
**InfluxDB Configuration:**
```
URL: https://us-east-1-1.aws.cloud2.influxdata.com
Org: your_organization
Bucket: fire-detection
Measurement: fire_detection
```

**PT-BR:**
```
┌──────────────────────────────────────────────────────────┐
│                    CAMADA DE SENSORES                      │
├──────────────────────────────────────────────────────────┤
│  Wokwi ESP32 (Simulado)        OpenWeather API (Real)      │
│  • Temperatura                 • Temperatura               │
│  • Umidade                     • Umidade                   │
│  • Sensor de Fumaça            • Velocidade do vento       │
│  • Sensor de Chama             • Condições climáticas      │
└───────────────┬──────────────────────┬─────────────────────┘
                │                      │
                │ MQTT (HiveMQ)        │ HTTP/REST
                │                      │
┌───────────────▼──────────────────────▼─────────────────────┐
│                   CAMADA DE PROCESSAMENTO                  │
├──────────────────────────────────────────────────────────┤
│                      Node-RED                              │
│  • Recebe dados MQTT do ESP32                              │
│  • Consulta clima em tempo real (3 estados)                │
│  • Algoritmo de análise de risco                           │
│  • Formatação de dados para InfluxDB                       │
│  • Geração de alertas WhatsApp                             │
└───────────────┬──────────────────────┬─────────────────────┘
                │                      │
                │ InfluxDB Line        │ WhatsApp API
                │ Protocol             │
┌───────────────▼──────────────────────▼─────────────────────┐
│                    CAMADA DE SAÍDA                         │
├──────────────────────────────────────────────────────────┤
│  InfluxDB Cloud      Grafana Cloud      WhatsApp           │
│  • Armazenamento     • Dashboards       • Alertas          │
│  • Séries temporais  • Visualização     • Crítico          │
│  • Análises          • Análises         • Alto             │
└──────────────────────────────────────────────────────────┘
```
**Configuração InfluxDB:**
```
URL: https://us-east-1-1.aws.cloud2.influxdata.com
Org: sua_organizacao
Bucket: fire-detection
Measurement: fire_detection
```
</div>

### 🧪 Fire Risk Algorithm | Algoritmo de Risco de Incêndio

<div align="left">

**EN:** The system calculates a risk score (0-100) based on 5 factors:

**PT-BR:** O sistema calcula uma pontuação de risco (0-100) baseado em 5 fatores:

</div>

<div align="center">

| Factor / Fator | Weight / Peso | Critical / Crítico | High / Alto | Medium / Médio | Low / Baixo |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 💨 Smoke Level / Nível de Fumaça | 40% | >600 ppm (+40) | 400-600 (+30) | 200-400 (+15) | <200 (+0) |
| 🌡️ Internal Temp / Temp Interna | 25% | >50°C (+25) | 40-50°C (+20) | 35-40°C (+10) | <35°C (+0) |
| 🌡️ External Temp / Temp Externa | 15% | >35°C (+15) | 30-35°C (+10) | 25-30°C (+5) | <25°C (+0) |
| 💧 Avg Humidity / Umidade Média | 10% | <30% (+10) | - | 30-50% (+5) | >50% (+0) |
| 🌬️ Wind Speed / Velocidade Vento | 10% | >40 km/h (+10) | - | 20-40 (+5) | <20 (+0) |

</div>

### 📊 Risk Classification | Classificação de Risco

<div align="center">

| Score / Pontuação | Level / Nível | Action EN | Ação PT-BR |
|:---:|:---:|:---|:---|
| 70-100 | 🔴 CRITICAL / CRÍTICO | Evacuation + Fire Dept (193) | Evacuação + Bombeiros (193) |
| 50-69 | 🟠 HIGH / ALTO | Maximum attention + Authorities | Atenção máxima + Autoridades |
| 30-49 | 🟡 MEDIUM / MÉDIO | Intensive monitoring | Monitoramento intensivo |
| 0-29 | 🟢 LOW / BAIXO | Continuous monitoring | Monitoramento contínuo |

</div>

<p align="center">
  <i>⚠️ WhatsApp alerts are automatically sent when score ≥ 50</i><br>
  <i>⚠️ Alertas WhatsApp são enviados automaticamente quando pontuação ≥ 50</i>
</p>

### 🛠️ Technologies | Tecnologias

<div align="center">
  <img src="https://img.shields.io/badge/Node.js-339933?style=for-the-badge&logo=nodedotjs&logoColor=white" alt="Node.js"/>
  <img src="https://img.shields.io/badge/MQTT-660066?style=for-the-badge&logo=mqtt&logoColor=white" alt="MQTT"/>
  <img src="https://img.shields.io/badge/InfluxDB-22ADF6?style=for-the-badge&logo=influxdb&logoColor=white" alt="InfluxDB"/>
  <img src="https://img.shields.io/badge/Grafana-F46800?style=for-the-badge&logo=grafana&logoColor=white" alt="Grafana"/>
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino"/>
</div>

<div align="center">

| Category / Categoria | Technology / Tecnologia | Description EN | Descrição PT-BR |
|:---:|:---:|:---|:---|
| **Hardware/Simulation** | Wokwi, ESP32, DHT22, MQ-2 | Online ESP32 simulator with sensors | Simulador ESP32 online com sensores |
| **Backend** | Node-RED, MQTT, OpenWeather API | IoT flow orchestration | Orquestração de fluxos IoT |
| **Database** | InfluxDB Cloud | Time-series database | Banco de dados de séries temporais |
| **Visualization** | Grafana Cloud | Dashboards and analytics | Dashboards e análises |
| **Notifications** | WhatsApp API (CallMeBot/Twilio) | Real-time alerts | Alertas em tempo real |

</div>

### 📁 Project Structure | Estrutura do Projeto

```
iot-risk-prevention/
├── 📄 README.md                 # This file | Este arquivo
├── 📄 SETUP.md                  # Setup guide | Guia de configuração
├── 📄 flows.json                # Node-RED flow | Fluxo Node-RED
├── 📂 wokwi/
│   ├── esp32-code.ino          # Arduino code | Código Arduino
│   ├── diagram.json            # Circuit diagram | Diagrama do circuito
│   └── libraries.txt           # Required libraries | Bibliotecas necessárias
├── 📂 grafana/
│   ├── GRAFANA_DASHBOARDS.md   # Dashboard guide | Guia de dashboards
│   └── QUERIES.md              # Flux queries | Queries Flux
├── 📂 docs/
│   └── ARCHITECTURE.md         # System architecture | Arquitetura do sistema
└── 📄 .gitignore
```

### 🚀 Quick Start | Início Rápido

<div align="left">

**EN:**
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

**PT-BR:**
```bash
# 1. Clone o repositório
git clone https://github.com/matheussricardoo/iot-risk-prevention.git

# 2. Instale o Node-RED
npm install -g --unsafe-perm node-red

# 3. Instale as dependências
cd ~/.node-red
npm install node-red-node-openweathermap node-red-contrib-influxdb node-red-contrib-whatsapp-cmb

# 4. Inicie o Node-RED
node-red

# 5. Importe o flows.json (http://localhost:1880)
# Menu → Import → Clipboard → Cole o conteúdo de flows.json → Deploy
```

</div>

<p align="center">
  <b>📚 Complete guide available at | Guia completo disponível em:</b><br>
  <a href="./SETUP.md">SETUP.md</a> | <a href="./docs/ARCHITECTURE.md">ARCHITECTURE.md</a>
</p>

### 🧪 Testing the System | Testando o Sistema

<div align="center">

| Test / Teste | Temperature / Temperatura | Humidity / Umidade | Smoke / Fumaça | Expected / Esperado |
|:---:|:---:|:---:|:---:|:---:|
| **Normal Conditions / Condições Normais** | 28°C | 65% | 120 ppm | 🟢 LOW RISK / RISCO BAIXO |
| **High Risk / Risco Alto** | 42°C | 35% | 450 ppm | 🟠 HIGH RISK + WhatsApp Alert |
| **Critical Fire / Incêndio Crítico** | 55°C | 20% | 750 ppm | 🔴 CRITICAL + Urgent Alert |

</div>

### 📊 Grafana Dashboard Panels | Painéis do Dashboard Grafana

<div align="center">

| Panel / Painel | Type / Tipo | Description EN | Descrição PT-BR |
|:---:|:---:|:---|:---|
| 1️⃣ | Gauges | Current metrics (temp, humidity, smoke, risk) | Métricas atuais (temp, umidade, fumaça, risco) |
| 2️⃣ | Stats | Risk score by state (AM, MS, MG) | Pontuação de risco por estado (AM, MS, MG) |
| 3️⃣ | Time Series | Historical metrics (last 6 hours) | Métricas históricas (últimas 6 horas) |
| 4️⃣ | Table | Latest records (all states) | Últimos registros (todos os estados) |

</div>

### 📱 WhatsApp Alert Example | Exemplo de Alerta WhatsApp

<div align="center">

```
🔴 *FIRE RISK ALERT: CRITICAL* 🔴
🔴 *ALERTA DE RISCO DE INCÊNDIO: CRÍTICO* 🔴

🔥 *RISK SCORE / PONTUAÇÃO DE RISCO: 85/100*

──────────────────
📍 *LOCATION / LOCALIZAÇÃO*
🏙️ City / Cidade: Manaus
🗺️ State / Estado: Amazonas (AM)

──────────────────
📊 *IoT SENSOR DATA / DADOS DO SENSOR*
🌡️ Temperature / Temperatura: 55.0°C
💧 Humidity / Umidade: 20.0%
💨 Smoke / Fumaça: 750 ppm

──────────────────
🌦️ *REAL WEATHER CONDITIONS / CONDIÇÕES REAIS*
🌡️ Temperature / Temperatura: 38.5°C
💧 Humidity / Umidade: 25.0%
🌬️ Wind / Vento: 45.2 km/h

──────────────────
⏰ 2025-10-31 16:42:09

🚨 *IMMEDIATE ACTION REQUIRED!*
🚨 *AÇÃO IMEDIATA NECESSÁRIA!*
🚒 Fire Department / Bombeiros: 193
```

</div>

### 🛡️ Security Best Practices | Boas Práticas de Segurança

<div align="center">

| Practice / Prática | Description EN | Descrição PT-BR |
|:---:|:---|:---|
| ✅ | Use environment variables for tokens/API keys | Use variáveis de ambiente para tokens/chaves API |
| ✅ | Never commit credentials to Git | Nunca commite credenciais no Git |
| ✅ | Use HTTPS for all external connections | Use HTTPS para todas as conexões externas |
| ✅ | Implement rate limiting on APIs | Implemente limitação de taxa nas APIs |
| ✅ | Configure data retention in InfluxDB | Configure retenção de dados no InfluxDB |
| ✅ | Enable authentication in Node-RED | Habilite autenticação no Node-RED |
| ✅ | Use TLS/SSL for MQTT broker (production) | Use TLS/SSL no broker MQTT (produção) |

</div>

### 📈 Future Improvements | Melhorias Futuras

<div align="center">

| Priority / Prioridade | Feature EN | Funcionalidade PT-BR |
|:---:|:---|:---|
| **Short Term / Curto Prazo** | Add more Brazilian states | Adicionar mais estados brasileiros |
| **Short Term / Curto Prazo** | Telegram integration | Integração com Telegram |
| **Medium Term / Médio Prazo** | Machine Learning for risk prediction | Machine Learning para predição de risco |
| **Medium Term / Médio Prazo** | NASA FIRMS satellite integration | Integração com satélites NASA FIRMS |
| **Long Term / Longo Prazo** | Real sensor network (not simulated) | Rede de sensores reais (não simulados) |
| **Long Term / Longo Prazo** | Civil Defense integration | Integração com Defesa Civil |

</div>

### 🤝 Contributing | Contribuindo

<div align="left">

**EN:** This is an academic project. Contributions are welcome!

**PT-BR:** Este é um projeto acadêmico. Contribuições são bem-vindas!

</div>

```bash
# Fork the project | Fork o projeto
# Create a branch | Crie uma branch
git checkout -b feature/nova-feature

# Commit your changes | Commit suas mudanças
git commit -m 'Add new feature'

# Push to the branch | Push para a branch
git push origin feature/nova-feature

# Open a Pull Request | Abra um Pull Request
```

### 📜 License | Licença

<p align="center">
  <a href="LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License: MIT">
  </a>
  <br>
  <i>This project is licensed under the MIT License. See the <a href="LICENSE">LICENSE</a> file for details.</i><br>
  <i>Este projeto está licenciado sob a Licença MIT. Veja o arquivo <a href="LICENSE">LICENSE</a> para mais detalhes.</i>
</p>

### 🙏 Acknowledgments | Agradecimentos

<div align="center">

| Platform / Plataforma | Thanks for / Obrigado por |
|:---:|:---|
| **Wokwi** | Excellent simulation platform / Excelente plataforma de simulação |
| **InfluxData** | Free InfluxDB Cloud / InfluxDB Cloud gratuito |
| **Grafana Labs** | Free Grafana Cloud / Grafana Cloud gratuito |
| **OpenWeather** | Weather API / API meteorológica |
| **HiveMQ** | Public MQTT broker / Broker MQTT público |

</div>

---

<p align="center">
  <b>⚠️ WARNING | AVISO ⚠️</b><br>
  <i>This is an educational monitoring system. In real emergencies, always call the fire department (193) and follow official safety protocols.</i><br>
  <i>Este é um sistema de monitoramento educacional. Em casos reais de emergência, sempre acione os bombeiros (193) e siga protocolos oficiais de segurança.</i>
</p>

<p align="center">
  <b>🔥 Wildfire prevention saves lives and preserves the environment! 🌳</b><br>
  <b>🔥 Prevenção de queimadas salva vidas e preserva o meio ambiente! 🌳</b>
</p>

### 👨‍💻 Authors | Autores

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
