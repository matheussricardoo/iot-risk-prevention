/*
 * ========================================
 * DISPOSITIVO 1: ESP32 COM SENSORES
 * Sistema IoT de Detecção de Queimadas
 * ========================================
 * 
 * Hardware:
 * - ESP32
 * - DHT22 (Temperatura e Umidade)
 * - MQ-2 (Sensor de Fumaça)
 * 
 * Função:
 * - Coleta dados dos sensores a cada 10 segundos
 * - Publica dados via MQTT no broker HiveMQ
 * - LED verde indica status OK
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
#define MQTT_TOPIC_DATA "iot/fire-detection/sensor-data"
#define MQTT_TOPIC_STATUS "iot/fire-detection/status"
#define MQTT_CLIENT_ID "ESP32-Sensores-Wokwi"

// ==========================================
// CONFIGURAÇÕES DE HARDWARE
// ==========================================
#define DHT_PIN 15          // DHT22
#define SMOKE_PIN 34        // MQ-2 (analógico)
#define LED_STATUS_PIN 2    // LED verde de status

#define DHT_TYPE DHT22

// ==========================================
// CONFIGURAÇÕES DE TIMING
// ==========================================
const unsigned long PUBLISH_INTERVAL = 10000;  // 10 segundos

// ==========================================
// OBJETOS GLOBAIS
// ==========================================
DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastPublish = 0;

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("  DISPOSITIVO 1: ESP32 COM SENSORES");
  Serial.println("========================================");
  
  // Configurar pinos
  pinMode(LED_STATUS_PIN, OUTPUT);
  digitalWrite(LED_STATUS_PIN, LOW);
  
  // Iniciar DHT22
  Serial.println("Inicializando DHT22...");
  dht.begin();
  delay(2000);
  
  // Conectar WiFi
  connectWiFi();
  
  // Configurar MQTT
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setKeepAlive(60);
  
  Serial.println("Sistema pronto!");
  Serial.println("========================================\n");
  
  // LED pisca 3x para indicar inicialização
  for(int i=0; i<3; i++) {
    digitalWrite(LED_STATUS_PIN, HIGH);
    delay(200);
    digitalWrite(LED_STATUS_PIN, LOW);
    delay(200);
  }
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================
void loop() {
  // Manter conexão MQTT
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
  mqtt.loop();
  
  // Publicar dados periodicamente
  if (millis() - lastPublish >= PUBLISH_INTERVAL) {
    publishSensorData();
    lastPublish = millis();
  }
  
  // Piscar LED a cada segundo (heartbeat)
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink >= 1000) {
    digitalWrite(LED_STATUS_PIN, !digitalRead(LED_STATUS_PIN));
    lastBlink = millis();
  }
}

// ==========================================
// CONECTAR WIFI
// ==========================================
void connectWiFi() {
  Serial.print("Conectando WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ==========================================
// RECONECTAR MQTT
// ==========================================
void reconnectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando MQTT...");
    
    if (mqtt.connect(MQTT_CLIENT_ID)) {
      Serial.println(" OK!");
      
      // Publicar status online
      mqtt.publish(MQTT_TOPIC_STATUS, "{\"device\":\"sensores\",\"status\":\"online\"}", true);
      
    } else {
      Serial.print(" Falha (rc=");
      Serial.print(mqtt.state());
      Serial.println(") - Tentando novamente em 5s");
      delay(5000);
    }
  }
}

// ==========================================
// PUBLICAR DADOS DOS SENSORES
// ==========================================
void publishSensorData() {
  Serial.println("\n--- Coletando Dados ---");
  
  // Ler sensores
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int smokeRaw = analogRead(SMOKE_PIN);
  int smokePPM = map(smokeRaw, 0, 4095, 50, 1000);
  smokePPM = constrain(smokePPM, 50, 1000);
  
  // Validar leituras
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("ERRO ao ler DHT22!");
    return;
  }
  
  // Exibir no Serial
  Serial.printf("Temperatura: %.1f°C\n", temperature);
  Serial.printf("Umidade: %.1f%%\n", humidity);
  Serial.printf("Fumaça: %d ppm\n", smokePPM);
  
  // Criar JSON
  StaticJsonDocument<256> doc;
  doc["device"] = "esp32-wokwi";
  doc["temperature"] = round(temperature * 10) / 10.0;
  doc["humidity"] = round(humidity * 10) / 10.0;
  doc["smoke"] = smokePPM;
  doc["alert"] = (temperature > 40 || smokePPM > 400);
  doc["timestamp"] = millis();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Publicar MQTT
  if (mqtt.publish(MQTT_TOPIC_DATA, jsonString.c_str())) {
    Serial.println("✓ Dados publicados!");
    Serial.println(jsonString);
  } else {
    Serial.println("✗ Falha ao publicar!");
  }
  
  Serial.println("------------------------\n");
}
