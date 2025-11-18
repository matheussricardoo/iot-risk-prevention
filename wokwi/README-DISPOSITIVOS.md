<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=2%20Dispositivos%20ESP32&fontSize=50&fontColor=fff&animation=twinkling&fontAlignY=40&desc=Sensores%20%2B%20Atuadores%20%7C%20Guia%20Completo&descAlignY=60&descSize=18">

# 🔥 Sistema IoT de Detecção de Queimadas - 2 Dispositivos ESP32

</div>

## 📋 Estrutura do Projeto

Este projeto utiliza **2 dispositivos ESP32 separados** conforme requisito acadêmico:

### **Dispositivo 1: ESP32 com SENSORES** 
- **Hardware:**
  - DHT22 (Temperatura e Umidade)
  - MQ-2 (Sensor de Fumaça - simulado com joystick analógico)
  - LED Verde (Status)

- **Função:**
  - Coleta dados dos sensores a cada 10 segundos
  - Publica dados via MQTT no tópico `iot/fire-detection/sensor-data`
  - LED pisca como heartbeat

### **Dispositivo 2: ESP32 com ATUADORES**
- **Hardware:**
  - 4 LEDs (Verde, Amarelo, Laranja, Vermelho)
  - Buzzer
  - Display LCD 16x2 I2C
  - Laser (alerta crítico)

- **Função:**
  - Recebe comandos via MQTT do Node-RED no tópico `iot/fire-detection/alerts`
  - Aciona LEDs conforme nível de risco
  - Ativa buzzer em alertas ALTO/CRÍTICO (10 segundos)
  - Exibe informações no LCD
  - Aciona laser em situação CRÍTICA

---

## 🚀 Como Usar no Wokwi

### **Passo 1: Criar Dispositivo 1 (Sensores)**

1. Acesse https://wokwi.com
2. Crie novo projeto "ESP32"
3. Cole o código de `esp32-sensores.ino`
4. Substitua o `diagram.json` pelo conteúdo de `diagram-sensores.json`
5. Adicione as bibliotecas de `libraries-sensores.txt`
6. Clique em "Start Simulation"
7. **IMPORTANTE:** Deixe rodando para publicar dados continuamente

### **Passo 2: Criar Dispositivo 2 (Atuadores)**

1. Abra **nova aba** no navegador
2. Acesse https://wokwi.com novamente
3. Crie novo projeto "ESP32"
4. Cole o código de `esp32-atuadores.ino`
5. Use o `diagram-atuadores.json` existente
6. Adicione as bibliotecas de `libraries-atuadores.txt`
7. Clique em "Start Simulation"

---

## 🔧 Configuração do Node-RED

### **Atualizar Tópico MQTT**

O Node-RED precisa estar configurado para:

**RECEBER dados do Dispositivo 1:**
- Tópico: `iot/fire-detection/sensor-data`
- Broker: `broker.hivemq.com:1883`

**ENVIAR comandos para o Dispositivo 2:**
- Tópico: `iot/fire-detection/alerts`
- Broker: `broker.hivemq.com:1883`

### **Fluxo de Dados**

```
┌─────────────────┐
│  ESP32 Sensores │ → Publica dados → MQTT
└─────────────────┘                     ↓
                                   Node-RED
                                        ↓
                              Calcula Risco
                                        ↓
                              Formata Alerta
                                        ↓
                                      MQTT
                                        ↓
                            ┌───────────────────┐
                            │ ESP32 Atuadores   │
                            │ - Liga LED        │
                            │ - Aciona Buzzer   │
                            │ - Atualiza LCD    │
                            │ - Liga Laser      │
                            └───────────────────┘
```

---

## 🧪 Testando o Sistema

### **Teste 1: Condições Normais**
1. No Wokwi (Sensores), ajuste o joystick para baixo (fumaça baixa)
2. Observe o LED Verde aceso no Dispositivo 2
3. LCD mostra: "Risco: BAIXO"

### **Teste 2: Risco ALTO**
1. Mova o joystick para cima (fumaça alta: ~600 ppm)
2. Node-RED calcula risco ≥ 50
3. Observe no Dispositivo 2:
   - LED Laranja acende
   - Buzzer toca por 10s (1000 Hz)
   - LCD mostra: "Risco: ALTO 55/100 AM"

### **Teste 3: Risco CRÍTICO**
1. Mova o joystick totalmente para cima (fumaça muito alta: ~900 ppm)
2. Node-RED calcula risco ≥ 70
3. Observe no Dispositivo 2:
   - LED Vermelho acende
   - Buzzer toca por 10s (2000 Hz)
   - **LASER ACENDE** (alerta crítico)
   - LCD mostra: "Risco: CRÍTICO 95/100 AM"

### **Teste 4: Múltiplos Estados**
1. No Node-RED, clique em "API: Auto a cada 1 minuto"
2. O sistema processa dados dos 3 estados (AM, MS, MG)
3. Dispositivo 2 exibe o estado com maior risco

---

## 📊 Monitoramento Serial

### **ESP32 Sensores (Dispositivo 1)**
```
========================================
  DISPOSITIVO 1: ESP32 COM SENSORES
========================================
Inicializando DHT22...
Conectando WiFi....
WiFi conectado!
IP: 192.168.1.100
Conectando MQTT... OK!
Subscrito em: iot/fire-detection/sensor-data
Sistema pronto!
========================================

--- Coletando Dados ---
Temperatura: 28.5°C
Umidade: 63.2%
Fumaça: 126 ppm
✓ Dados publicados!
{"device":"esp32-wokwi","temperature":28.5,"humidity":63.2,"smoke":126,"alert":false,"timestamp":10234}
------------------------
```

### **ESP32 Atuadores (Dispositivo 2)**
```
========================================
  DISPOSITIVO 2: ESP32 COM ATUADORES
========================================
Inicializando LCD...
Conectando WiFi....
WiFi conectado!
IP: 192.168.1.101
RSSI: -45 dBm
Conectando MQTT (broker.hivemq.com)... OK!
Subscrito em: iot/fire-detection/alerts
Sistema pronto!
Aguardando alertas via MQTT...
========================================

========================================
Mensagem recebida [iot/fire-detection/alerts]
Payload: {"sensor":{"device":"esp32-wokwi","temperature":55,"humidity":20,"smoke":750},"weather":{"temperature":38.2,"humidity":22.5,"windSpeed":25.3,"conditions":"céu limpo"},"location":{"city":"Manaus","state":"Amazonas","abbr":"AM","country":"Brazil"},"risk":{"score":95,"level":"CRÍTICO","color":"🔴","shouldAlert":true},"timestamp":"2025-11-18T18:42:33.123Z"}

--- DADOS DO ALERTA ---
Risco: CRÍTICO (95/100)
Estado: AM (Manaus)
-----------------------
LED: VERMELHO (Risco CRÍTICO)
LCD atualizado
BUZZER ATIVADO (2000 Hz por 10s)
🔴 LASER ATIVADO!
========================================
```

---

## ⚠️ Solução de Problemas

### **ESP32 Atuadores não conecta ao MQTT**

**Sintomas:**
- Serial Monitor mostra: "Falha (rc=-2)" ou "rc=-4"
- LCD exibe: "ERRO MQTT!"

**Soluções:**

1. **Verificar WiFi:**
   - Wokwi usa WiFi virtual automático
   - Certifique-se que a simulação está rodando

2. **Broker HiveMQ ocupado:**
   - Tente trocar o broker para `test.mosquitto.org`
   - Altere no código: `#define MQTT_BROKER "test.mosquitto.org"`

3. **Client ID duplicado:**
   - Se ambos ESP32 tiverem mesmo ID, um será desconectado
   - O código já usa IDs únicos: `ESP32-Sensores-Wokwi` e `ESP32-Atuadores-Wokwi`

4. **Timeout de conexão:**
   - Aguarde 10-15 segundos para primeira conexão
   - O código tenta 3 vezes com delay de 5s

5. **Reiniciar simulações:**
   - Pare ambas simulações
   - Inicie primeiro o Dispositivo 1 (Sensores)
   - Aguarde 10s
   - Inicie o Dispositivo 2 (Atuadores)

---

## 📱 Integração com Node-RED

### **Nó MQTT In (Receber Sensores)**
```
Tópico: iot/fire-detection/sensor-data
Broker: broker.hivemq.com:1883
QoS: 0
```

### **Nó MQTT Out (Enviar Alertas)**
```
Tópico: iot/fire-detection/alerts
Broker: broker.hivemq.com:1883
QoS: 0
Retain: false
```

### **Payload de Alerta (Node-RED → Atuadores)**

O Node-RED deve enviar JSON neste formato:

```json
{
  "sensor": {
    "device": "esp32-wokwi",
    "temperature": 55.0,
    "humidity": 20.0,
    "smoke": 750
  },
  "weather": {
    "temperature": 38.2,
    "humidity": 22.5,
    "windSpeed": 25.3,
    "conditions": "céu limpo"
  },
  "location": {
    "city": "Manaus",
    "state": "Amazonas",
    "abbr": "AM",
    "country": "Brazil"
  },
  "risk": {
    "score": 95,
    "level": "CRÍTICO",
    "color": "🔴",
    "shouldAlert": true
  },
  "timestamp": "2025-11-18T18:42:33.123Z"
}
```

---

## 🎯 Níveis de Risco e Atuação

| Pontuação | Nível | LED | Buzzer | Laser | Duração Buzzer |
|-----------|-------|-----|--------|-------|----------------|
| 0-29 | BAIXO | 🟢 Verde | OFF | OFF | - |
| 30-49 | MÉDIO | 🟡 Amarelo | OFF | OFF | - |
| 50-69 | ALTO | 🟠 Laranja | 1000 Hz | OFF | 10 segundos |
| 70-100 | CRÍTICO | 🔴 Vermelho | 2000 Hz | **ON** | 10 segundos |

---

## 📦 Arquivos do Projeto

```
wokwi/
├── esp32-sensores.ino          # Código Dispositivo 1
├── esp32-atuadores.ino         # Código Dispositivo 2
├── diagram-sensores.json       # Circuito Wokwi Sensores
├── diagram-atuadores.json      # Circuito Wokwi Atuadores
├── libraries-sensores.txt      # Bibliotecas Sensores
├── libraries-atuadores.txt     # Bibliotecas Atuadores
└── README-DISPOSITIVOS.md      # Este arquivo
```

---

## ✅ Checklist de Funcionamento

### **Dispositivo 1 (Sensores) ✓**
- [x] WiFi conecta
- [x] MQTT conecta ao broker
- [x] DHT22 lê temperatura/umidade
- [x] MQ-2 lê fumaça (joystick)
- [x] Publica dados a cada 10s
- [x] LED pisca (heartbeat)

### **Dispositivo 2 (Atuadores) ✓**
- [x] WiFi conecta
- [x] MQTT conecta ao broker
- [x] Subscreve no tópico correto
- [x] LCD inicializa
- [x] Recebe mensagens do Node-RED
- [x] LEDs acendem conforme risco
- [x] Buzzer toca em alertas
- [x] Laser ativa em crítico
- [x] LCD atualiza com dados

---

## 🎓 Atendimento aos Requisitos Acadêmicos

✅ **Uso de 2 dispositivos simulados**  
✅ **Dispositivo 1: Contém APENAS sensores**  
✅ **Dispositivo 2: Contém APENAS atuadores**  
✅ **Comunicação via MQTT entre dispositivos (através do Node-RED)**  
✅ **Integração com API externa (OpenWeather)**  
✅ **Armazenamento em banco de dados (InfluxDB)**  
✅ **Visualização em dashboard (Grafana)**  
✅ **Alertas automáticos (WhatsApp)**  

---

## 🚀 Próximos Passos

1. ✅ Criar ambos dispositivos no Wokwi
2. ✅ Testar conexão MQTT de cada um
3. ✅ Configurar Node-RED para receber/enviar dados
4. ⏳ Testar fluxo completo end-to-end
5. ⏳ Gravar vídeo de demonstração
6. ⏳ Documentar no relatório final

---

**Projeto desenvolvido para disciplina de IoT**  
**Instituição: [Sua Instituição]**  
**Data: Novembro 2025**

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>
