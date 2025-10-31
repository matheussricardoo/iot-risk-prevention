/*
 * ========================================
 * Sistema IoT de Detecção de Queimadas
 * ESP32 + DHT22 + Sensor de Fumaça
 * ========================================
 * 
 * Autor: Projeto IoT Prevenção de Queimadas
 * Plataforma: Wokwi (simulação)
 * Microcontrolador: ESP32
 * 
 * Funcionalidades:
 * - Leitura de temperatura e umidade (DHT22)
 * - Monitoramento de fumaça (MQ-2 simulado)
 * - Detecção de chamas (sensor IR)
 * - Envio de dados via MQTT (HiveMQ público)
 * - Alertas locais (LED + Buzzer)
 * 
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// ==========================================
// CONFIGURAÇÕES DE REDE
// ==========================================
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""

// ==========================================
// CONFIGURAÇÕES MQTT
// ==========================================
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_TOPIC "iot/fire-detection/esp32-wokwi"
#define MQTT_CLIENT_ID_PREFIX "ESP32-Fire-"

// ==========================================
// CONFIGURAÇÕES DE HARDWARE
// ==========================================
// Pinos dos Sensores
#define DHT_PIN 15          // DHT22 - Temperatura e Umidade
#define SMOKE_PIN 34        // Sensor de Fumaça (analógico)
#define FLAME_PIN 35        // Sensor de Chama (digital)

// Pinos de Saída
#define BUZZER_PIN 25       // Buzzer de alerta
#define LED_RED_PIN 2       // LED vermelho (alerta)
#define LED_GREEN_PIN 4     // LED verde (status ok)

// ==========================================
// CONFIGURAÇÕES DO SENSOR DHT
// ==========================================
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// ==========================================
// CONFIGURAÇÕES DE TIMING
// ==========================================
const unsigned long PUBLISH_INTERVAL = 10000;  // 10 segundos
const unsigned long WIFI_TIMEOUT = 10000;      // 10 segundos
const unsigned long MQTT_RECONNECT_DELAY = 5000; // 5 segundos

// ==========================================
// VARIÁVEIS GLOBAIS
// ==========================================
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastPublish = 0;
unsigned long lastWifiCheck = 0;

// Limiares de alerta
const float TEMP_THRESHOLD = 40.0;     // 40°C
const int SMOKE_THRESHOLD = 400;       // 400 ppm
const float HUMIDITY_LOW = 30.0;       // 30%

// ==========================================
// PROTÓTIPOS DE FUNÇÕES
// ==========================================
void connectWiFi();
void reconnectMQTT();
void publishSensorData();
void checkAlertConditions(float temp, float humidity, int smoke, bool flame);
void soundBuzzer(int duration);
void blinkLED(int pin, int times);

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("  Sistema IoT Detecção de Queimadas");
  Serial.println("  ESP32 + Sensores + MQTT");
  Serial.println("========================================");
  
  // Configurar pinos de saída
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Configurar pinos de entrada
  pinMode(FLAME_PIN, INPUT);
  
  // Estado inicial
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Iniciar DHT22
  Serial.println("🌡️  Inicializando DHT22...");
  dht.begin();
  delay(2000); // Aguardar estabilização
  
  // Conectar WiFi
  connectWiFi();
  
  // Configurar MQTT
  Serial.println("📡 Configurando MQTT...");
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setKeepAlive(60);
  
  // Blink de inicialização
  blinkLED(LED_GREEN_PIN, 3);
  
  Serial.println("✅ Sistema iniciado com sucesso!");
  Serial.println("========================================\n");
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================
void loop() {
  // Verificar conexão MQTT
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
  mqtt.loop();
  
  // Publicar dados periodicamente
  if (millis() - lastPublish >= PUBLISH_INTERVAL) {
    publishSensorData();
    lastPublish = millis();
  }
  
  // Verificar WiFi periodicamente
  if (millis() - lastWifiCheck >= 30000) { // A cada 30s
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("⚠️  WiFi desconectado! Reconectando...");
      connectWiFi();
    }
    lastWifiCheck = millis();
  }
}

// ==========================================
// FUNÇÃO: CONECTAR WIFI
// ==========================================
void connectWiFi() {
  Serial.print("🌐 Conectando WiFi");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  unsigned long startAttempt = millis();
  
  while (WiFi.status() != WL_CONNECTED && 
         millis() - startAttempt < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
    blinkLED(LED_GREEN_PIN, 1);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi conectado!");
    Serial.print("📡 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("📶 RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    digitalWrite(LED_GREEN_PIN, HIGH);
  } else {
    Serial.println("\n❌ Falha ao conectar WiFi!");
    digitalWrite(LED_RED_PIN, HIGH);
  }
}

// ==========================================
// FUNÇÃO: RECONECTAR MQTT
// ==========================================
void reconnectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("🔌 Conectando MQTT broker (");
    Serial.print(MQTT_BROKER);
    Serial.print(")...");
    
    // Gerar Client ID único
    String clientId = MQTT_CLIENT_ID_PREFIX;
    clientId += String(random(0xffff), HEX);
    
    if (mqtt.connect(clientId.c_str())) {
      Serial.println(" ✅ Conectado!");
      
      // Publicar mensagem de online
      String onlineMsg = "{\"status\":\"online\",\"device\":\"" + clientId + "\"}";
      mqtt.publish("iot/fire-detection/status", onlineMsg.c_str(), true);
      
    } else {
      Serial.print(" ❌ Falha! (rc=");
      Serial.print(mqtt.state());
      Serial.println(")");
      Serial.println("Tentando novamente em 5 segundos...");
      
      blinkLED(LED_RED_PIN, 3);
      delay(MQTT_RECONNECT_DELAY);
    }
  }
}

// ==========================================
// FUNÇÃO: PUBLICAR DADOS DOS SENSORES
// ==========================================
void publishSensorData() {
  Serial.println("\n========================================");
  Serial.println("📊 Coletando dados dos sensores...");
  
  // Ler DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Ler sensores analógicos/digitais
  int smokeRaw = analogRead(SMOKE_PIN);
  int flameDetected = digitalRead(FLAME_PIN);
  
  // Validar leituras do DHT22
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ Erro ao ler DHT22!");
    blinkLED(LED_RED_PIN, 2);
    return;
  }
  
  // Converter leitura de fumaça para PPM (0-1000)
  int smokePPM = map(smokeRaw, 0, 4095, 0, 1000);
  smokePPM = constrain(smokePPM, 0, 1000);
  
  // Determinar estado de alerta
  bool alertStatus = (temperature > TEMP_THRESHOLD || 
                      smokePPM > SMOKE_THRESHOLD || 
                      humidity < HUMIDITY_LOW ||
                      flameDetected == HIGH);
  
  // Exibir leituras no Serial
  Serial.println("┌─────────────────────────────┐");
  Serial.println("│   LEITURAS DOS SENSORES     │");
  Serial.println("├─────────────────────────────┤");
  Serial.print("│ 🌡️  Temperatura:  ");
  Serial.print(temperature, 1);
  Serial.println(" °C   │");
  Serial.print("│ 💧 Umidade:       ");
  Serial.print(humidity, 1);
  Serial.println(" %    │");
  Serial.print("│ 💨 Fumaça:        ");
  Serial.print(smokePPM);
  Serial.println(" ppm  │");
  Serial.print("│ 🔥 Chama:         ");
  Serial.println(flameDetected == HIGH ? "SIM      │" : "NÃO      │");
  Serial.print("│ ⚠️  Alerta:       ");
  Serial.println(alertStatus ? "ATIVO    │" : "NORMAL   │");
  Serial.println("└─────────────────────────────┘");
  
  // Criar payload JSON
  StaticJsonDocument<300> doc;
  
  doc["device"] = "esp32-wokwi";
  doc["temperature"] = round(temperature * 10) / 10.0;
  doc["humidity"] = round(humidity * 10) / 10.0;
  doc["smoke"] = smokePPM;
  doc["flame"] = (flameDetected == HIGH);
  doc["alert"] = alertStatus;
  doc["rssi"] = WiFi.RSSI();
  doc["timestamp"] = millis();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Publicar no MQTT
  Serial.print("📤 Publicando em: ");
  Serial.println(MQTT_TOPIC);
  
  if (mqtt.publish(MQTT_TOPIC, jsonString.c_str())) {
    Serial.println("✅ Dados publicados com sucesso!");
    Serial.print("📦 Payload: ");
    Serial.println(jsonString);
    
    // Feedback visual
    blinkLED(LED_GREEN_PIN, 1);
    
    // Verificar condições de alerta
    checkAlertConditions(temperature, humidity, smokePPM, flameDetected == HIGH);
    
  } else {
    Serial.println("❌ Falha ao publicar dados!");
    blinkLED(LED_RED_PIN, 2);
  }
  
  Serial.println("========================================\n");
}

// ==========================================
// FUNÇÃO: VERIFICAR CONDIÇÕES DE ALERTA
// ==========================================
void checkAlertConditions(float temp, float humidity, int smoke, bool flame) {
  bool criticalAlert = false;
  
  Serial.println("🔍 Analisando condições...");
  
  // Verificar temperatura
  if (temp > TEMP_THRESHOLD) {
    Serial.print("⚠️  Temperatura alta: ");
    Serial.print(temp);
    Serial.println("°C");
    criticalAlert = true;
  }
  
  // Verificar fumaça
  if (smoke > SMOKE_THRESHOLD) {
    Serial.print("⚠️  Fumaça detectada: ");
    Serial.print(smoke);
    Serial.println(" ppm");
    criticalAlert = true;
  }
  
  // Verificar umidade baixa
  if (humidity < HUMIDITY_LOW) {
    Serial.print("⚠️  Umidade baixa: ");
    Serial.print(humidity);
    Serial.println("%");
    criticalAlert = true;
  }
  
  // Verificar chama
  if (flame) {
    Serial.println("🔥 CHAMA DETECTADA!");
    criticalAlert = true;
  }
  
  // Acionar alertas locais
  if (criticalAlert) {
    Serial.println("🚨 ALERTA CRÍTICO!");
    digitalWrite(LED_RED_PIN, HIGH);
    soundBuzzer(1000);
    delay(100);
    digitalWrite(LED_RED_PIN, LOW);
  } else {
    Serial.println("✅ Condições normais");
  }
}

// ==========================================
// FUNÇÃO: ACIONAR BUZZER
// ==========================================
void soundBuzzer(int duration) {
  tone(BUZZER_PIN, 1000, duration);
}

// ==========================================
// FUNÇÃO: PISCAR LED
// ==========================================
void blinkLED(int pin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(100);
    digitalWrite(pin, LOW);
    delay(100);
  }
}
