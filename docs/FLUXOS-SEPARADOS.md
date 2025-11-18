<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=2%20Fluxos%20Separados&fontSize=50&fontColor=fff&animation=twinkling&fontAlignY=40&desc=ESP32%20vs%20API%20%7C%20Dados%20Simulados%20vs%20Reais&descAlignY=60&descSize=18">

# 📊 Sistema com 2 Fluxos Separados

</div>

## 🎯 Problema Identificado

Você notou que as 3 cidades estavam com temperaturas muito parecidas, o que não faz sentido para dados "reais" da API. Isso acontecia porque:

1. **Fluxo antigo misturava** dados do ESP32 simulado com dados reais da API
2. O ESP32 publicava `temperature: 28` e esse valor era replicado para as 3 cidades
3. A API só estava atualizando **condições meteorológicas** (vento, condições), mas a temperatura ficava a do sensor

---

## ✅ Solução Implementada: 2 Fluxos Independentes

### **📍 ABA 1: "Detecção de Queimadas - Multi-Estado"**
**Fluxo ESP32 (Simulado)**

```
ESP32 Wokwi (MQTT)
    ↓
Parse JSON
    ↓
Salvar Dados Sensor
    ↓
Distribuir para Estados (AM, MS, MG)
    ↓
OpenWeather API (pega dados REAIS de cada cidade)
    ↓
Calcular Risco (COMBINA sensor simulado + clima real)
    ↓
InfluxDB (device='esp32-wokwi')
```

**Dados salvos:**
- `device: "esp32-wokwi"`
- `sensor_temp`: Temperatura do DHT22 simulado
- `sensor_humidity`: Umidade do DHT22 simulado
- `sensor_smoke`: Fumaça do MQ-2 simulado
- `weather_temp`: Temperatura REAL da cidade (OpenWeather)
- `weather_humidity`: Umidade REAL da cidade
- `weather_wind`: Vento REAL da cidade
- `risk_score`: Calculado com 5 fatores (sensor + clima)

---

### **📍 ABA 2: "API OpenWeather - Dados Reais (Separado)"** ⭐ **NOVA**
**Fluxo API Puro**

```
Timer (3 minutos)
    ↓
Criar Requisições (AM, MS, MG)
    ↓
OpenWeather API (3 chamadas independentes)
    ↓
Calcular Risco (APENAS dados meteorológicos)
    ↓
InfluxDB (device='api')
```

**Dados salvos:**
- `device: "api"` ⭐ **DIFERENTE**
- `sensor_temp`: Temperatura REAL da API (sem sensor)
- `sensor_humidity`: Umidade REAL da API
- `sensor_smoke`: 0 (API não tem sensor de fumaça)
- `weather_temp`: Mesma temperatura (redundante mas mantém estrutura)
- `weather_humidity`: Mesma umidade
- `weather_wind`: Vento REAL
- `risk_score`: Calculado APENAS com 3 fatores meteorológicos

---

## 🔥 Algoritmos de Risco Diferentes

### **Algoritmo ESP32 (5 fatores - 100 pontos)**
```
40% - Fumaça do sensor (MQ-2)
25% - Temperatura interna (DHT22)
15% - Temperatura externa (API)
10% - Umidade combinada (sensor + API)
10% - Vento (API)
```

### **Algoritmo API (3 fatores - 100 pontos)** ⭐ **NOVO**
```
50% - Temperatura externa (API)
30% - Umidade (API)
20% - Vento (API)
```

**Por que algoritmo diferente?**
- API não tem sensor de fumaça (principal indicador)
- Foco em condições climáticas que favorecem propagação de incêndios
- Mais conservador: precisa de condições extremas para alertar

---

## 📊 Comparação de Dados

### **Exemplo 1: Fluxo ESP32**
```json
{
  "device": "esp32-wokwi",
  "sensor_temp": 55,        // Do DHT22 simulado
  "sensor_humidity": 20,    // Do DHT22 simulado
  "sensor_smoke": 750,      // Do MQ-2 simulado
  "weather_temp": 38.2,     // API Manaus
  "weather_humidity": 45,   // API Manaus
  "weather_wind": 18,       // API Manaus
  "risk_score": 95,         // CRÍTICO (fumaça alta!)
  "state": "AM",
  "location": "Manaus"
}
```

### **Exemplo 2: Fluxo API (mesma hora)**
```json
{
  "device": "api",          // ⭐ Identificador diferente
  "sensor_temp": 38.2,      // Apenas temperatura da API
  "sensor_humidity": 45,    // Apenas umidade da API
  "sensor_smoke": 0,        // ⭐ Sem sensor de fumaça
  "weather_temp": 38.2,     // Mesma temperatura
  "weather_humidity": 45,   // Mesma umidade
  "weather_wind": 18,       // Mesmo vento
  "risk_score": 40,         // ⭐ MÉDIO (sem fumaça)
  "state": "AM",
  "location": "Manaus"
}
```

**Diferença:** Mesmo com temperatura alta (38.2°C), a API calcula risco MÉDIO (40) enquanto o ESP32 com fumaça alta calcula CRÍTICO (95).

---

## 🎨 Visualização no Grafana

### **Query para Dashboard ESP32:**
```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r._measurement == "fire_detection")
  |> filter(fn: (r) => r.device == "esp32-wokwi")
  |> filter(fn: (r) => r._field == "risk_score")
```

### **Query para Dashboard API:**
```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r._measurement == "fire_detection")
  |> filter(fn: (r) => r.device == "api")
  |> filter(fn: (r) => r._field == "risk_score")
```

### **Query para Comparação (ambos):**
```flux
from(bucket: "fire-detection")
  |> range(start: -24h)
  |> filter(fn: (r) => r._measurement == "fire_detection")
  |> filter(fn: (r) => r._field == "risk_score")
  |> group(columns: ["device", "state"])
```

---

## 🚀 Como Usar

### **Passo 1: Importar Novo Flow**
1. Abra Node-RED
2. Menu → Import → Clipboard
3. Cole o conteúdo do `flows.json` atualizado
4. Clique em "Import"
5. **Deploy**

### **Passo 2: Verificar Abas**
Você verá **2 abas** no Node-RED:
- ✅ "Detecção de Queimadas - Multi-Estado" (fluxo original ESP32)
- ✅ "API OpenWeather - Dados Reais (Separado)" ⭐ **NOVA**

### **Passo 3: Ativar Fluxo API**
1. Vá para a aba "API OpenWeather - Dados Reais (Separado)"
2. O timer já está configurado para **3 minutos**
3. Aguarde 5 segundos após deploy (primeira execução automática)
4. Verifique debug "Debug: API Data"

### **Passo 4: Verificar InfluxDB**
```sql
-- Ver dados ESP32
SELECT * FROM fire_detection WHERE device = 'esp32-wokwi' ORDER BY time DESC LIMIT 10

-- Ver dados API
SELECT * FROM fire_detection WHERE device = 'api' ORDER BY time DESC LIMIT 10

-- Comparar temperaturas das 3 cidades
SELECT state, weather_temp FROM fire_detection WHERE device = 'api' ORDER BY time DESC LIMIT 3
```

**Agora você verá temperaturas DIFERENTES para cada cidade!**

---

## 📈 Frequência de Coleta

### **Fluxo ESP32:**
- Depende da publicação MQTT do Wokwi
- Geralmente a cada 10 segundos
- Dados: sensor simulado + API real

### **Fluxo API:**
- **A cada 3 minutos** (configurável)
- 3 chamadas simultâneas (AM, MS, MG)
- **60 chamadas/hora** (20 por cidade)
- **1.440 chamadas/dia** (dentro do limite gratuito 1.000/dia da OpenWeather)

⚠️ **Ajuste recomendado:** Mudar para 5 minutos se estiver perto do limite da API.

---

## 🎯 Vantagens da Separação

### ✅ **Dados ESP32 (Simulado):**
- Simula sensores IoT reais
- Mostra como seria com hardware real
- Combina sensor + clima para análise completa
- Bom para demonstrações e testes

### ✅ **Dados API (Real):**
- 100% dados reais das 3 cidades
- Mostra condições climáticas verdadeiras
- Temperaturas DIFERENTES para cada cidade
- Bom para análise meteorológica real
- Pode ser usado para validar o algoritmo

### ✅ **Comparação:**
- Ver diferença entre dados simulados e reais
- Identificar padrões climáticos
- Validar precisão do sensor simulado
- Análise de correlação

---

## 🔍 Teste Rápido

### **Verificar se API está pegando dados DIFERENTES:**

1. Abra Node-RED
2. Vá para aba "API OpenWeather - Dados Reais (Separado)"
3. Clique no botão do inject "API: A cada 3 minutos"
4. Abra debug "Debug: API Data"
5. **Aguarde 30 segundos** (são 3 chamadas sequenciais)
6. Você verá **3 mensagens** com dados **DIFERENTES**:

```javascript
// Mensagem 1 (Manaus)
{
  location: { city: "Manaus", abbr: "AM" },
  weather: { temperature: 32.5, humidity: 78, windSpeed: 12 }
}

// Mensagem 2 (Campo Grande)
{
  location: { city: "Campo Grande", abbr: "MS" },
  weather: { temperature: 28.1, humidity: 55, windSpeed: 18 }
}

// Mensagem 3 (Belo Horizonte)
{
  location: { city: "Belo Horizonte", abbr: "MG" },
  weather: { temperature: 25.3, humidity: 62, windSpeed: 8 }
}
```

**✅ Temperaturas DIFERENTES confirmam que API está funcionando!**

---

## 📝 Resumo

| Característica | Fluxo ESP32 | Fluxo API |
|----------------|-------------|-----------|
| **Device Tag** | `esp32-wokwi` | `api` |
| **Fonte Temperatura** | DHT22 simulado | OpenWeather API |
| **Fonte Fumaça** | MQ-2 simulado | ❌ Não tem |
| **Fonte Clima** | OpenWeather API | OpenWeather API |
| **Algoritmo** | 5 fatores (100pts) | 3 fatores (100pts) |
| **Frequência** | ~10 segundos | 3 minutos |
| **Cidades** | 3 (com sensor único) | 3 (independentes) |
| **Uso** | Demonstração IoT | Dados reais |

---

**Agora você tem 2 dashboards completamente separados no Grafana! 🎉**

1. **Dashboard ESP32:** Mostra dados do simulador Wokwi
2. **Dashboard API:** Mostra dados meteorológicos reais das 3 cidades

Para criar os dashboards, use os filtros `device='esp32-wokwi'` e `device='api'` nas queries Flux!

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>

````
