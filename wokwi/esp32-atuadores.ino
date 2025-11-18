/*
 * ========================================
 * DISPOSITIVO 2: ESP32 COM ATUADORES
 * Sistema IoT de Detecção de Queimadas
 * ========================================
 * 
 * Hardware:
 * - ESP32
 * - 4 LEDs (Verde, Amarelo, Laranja, Vermelho)
 * - Buzzer
 * - Display LCD 16x2 I2C
 * - Laser (para alerta crítico)
 * 
 * Função:
 * - Recebe comandos via MQTT do Node-RED
 * - Aciona LEDs conforme nível de risco
 * - Ativa buzzer em alertas
 * - Exibe informações no LCD
 * - Aciona laser em situação crítica
 * 
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

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
#define MQTT_TOPIC_ALERTS "iot/fire-detection/alerts"
#define MQTT_TOPIC_STATUS "iot/fire-detection/actuator-status"
#define MQTT_CLIENT_ID "ESP32-Atuadores-Wokwi"

// ==========================================
// CONFIGURAÇÕES DE HARDWARE - ATUADORES
// ==========================================
// LEDs de Nível de Risco
#define LED_GREEN_PIN 13    // Risco BAIXO (0-29)
#define LED_YELLOW_PIN 12   // Risco MÉDIO (30-49)
#define LED_ORANGE_PIN 14   // Risco ALTO (50-69)
#define LED_RED_PIN 27      // Risco CRÍTICO (70-100)

// Alertas Sonoros/Visuais
#define BUZZER_PIN 26       // Buzzer
#define LASER_PIN 25        // Laser (alerta crítico)

// Display LCD I2C
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// ==========================================
// OBJETOS GLOBAIS
// ==========================================
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// Estado atual
String currentRiskLevel = "BAIXO";
int currentRiskScore = 0;
String currentState = "--";
bool buzzerActive = false;
unsigned long buzzerStartTime = 0;
const unsigned long BUZZER_DURATION = 10000; // 10 segundos

// ==========================================
// PROTÓTIPOS
// ==========================================
void connectWiFi();
void reconnectMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void processAlert(JsonDocument& doc);
void updateLEDs(int riskScore);
void updateLCD(String riskLevel, int riskScore, String state);
void activateBuzzer(int riskScore);
void deactivateAllAlerts();

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("  DISPOSITIVO 2: ESP32 COM ATUADORES");
  Serial.println("========================================");
  
  // Configurar pinos de saída
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_ORANGE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  
  // Estado inicial: todos desligados
  deactivateAllAlerts();
  
  // Inicializar LCD
  Serial.println("Inicializando LCD...");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema IoT");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  
  // Conectar WiFi
  connectWiFi();
  
  // Configurar MQTT
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  mqtt.setKeepAlive(60);
  
  // Conectar MQTT e subscrever
  reconnectMQTT();
  
  // Display inicial
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aguardando");
  lcd.setCursor(0, 1);
  lcd.print("dados...");
  
  Serial.println("Sistema pronto!");
  Serial.println("Aguardando alertas via MQTT...");
  Serial.println("========================================\n");
  
  // Teste de LEDs na inicialização
  Serial.println("Testando LEDs...");
  digitalWrite(LED_GREEN_PIN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  delay(300);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_ORANGE_PIN, HIGH);
  delay(300);
  digitalWrite(LED_ORANGE_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
  delay(300);
  digitalWrite(LED_RED_PIN, LOW);
  
  // Estado inicial: LED verde
  digitalWrite(LED_GREEN_PIN, HIGH);
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
  
  // Gerenciar buzzer (desligar após duração)
  if (buzzerActive && (millis() - buzzerStartTime >= BUZZER_DURATION)) {
    noTone(BUZZER_PIN);
    buzzerActive = false;
    Serial.println("Buzzer desativado (timeout)");
  }
  
  // Publicar status a cada 30 segundos
  static unsigned long lastStatusPublish = 0;
  if (millis() - lastStatusPublish >= 30000) {
    StaticJsonDocument<256> statusDoc;
    statusDoc["device"] = "atuadores";
    statusDoc["status"] = "online";
    statusDoc["currentRisk"] = currentRiskLevel;
    statusDoc["riskScore"] = currentRiskScore;
    statusDoc["state"] = currentState;
    
    String statusJson;
    serializeJson(statusDoc, statusJson);
    mqtt.publish(MQTT_TOPIC_STATUS, statusJson.c_str());
    
    lastStatusPublish = millis();
  }
}

// ==========================================
// CONECTAR WIFI
// ==========================================
void connectWiFi() {
  Serial.print("Conectando WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\nERRO: Falha ao conectar WiFi!");
    lcd.clear();
    lcd.print("ERRO WiFi!");
  }
}

// ==========================================
// RECONECTAR MQTT
// ==========================================
void reconnectMQTT() {
  int attempts = 0;
  
  while (!mqtt.connected() && attempts < 3) {
    Serial.print("Conectando MQTT (");
    Serial.print(MQTT_BROKER);
    Serial.print(")...");
    
    if (mqtt.connect(MQTT_CLIENT_ID)) {
      Serial.println(" OK!");
      
      // Subscrever no tópico de alertas
      if (mqtt.subscribe(MQTT_TOPIC_ALERTS)) {
        Serial.print("Subscrito em: ");
        Serial.println(MQTT_TOPIC_ALERTS);
      } else {
        Serial.println("ERRO ao subscrever!");
      }
      
      // Publicar status online
      mqtt.publish(MQTT_TOPIC_STATUS, "{\"device\":\"atuadores\",\"status\":\"online\"}", true);
      
    } else {
      Serial.print(" Falha (rc=");
      Serial.print(mqtt.state());
      Serial.println(")");
      Serial.println("Códigos de erro:");
      Serial.println("-4: Timeout conexão");
      Serial.println("-3: Conexão perdida");
      Serial.println("-2: Falha conexão");
      Serial.println("-1: Desconectado");
      Serial.println(" 0: Conectado");
      Serial.println(" 1: Protocolo incorreto");
      Serial.println(" 2: ID rejeitado");
      Serial.println(" 3: Servidor indisponível");
      Serial.println(" 4: Credenciais inválidas");
      Serial.println(" 5: Não autorizado");
      
      attempts++;
      if (attempts < 3) {
        Serial.println("Tentando novamente em 5s...");
        delay(5000);
      }
    }
  }
  
  if (!mqtt.connected()) {
    Serial.println("FALHA CRÍTICA: Não foi possível conectar ao MQTT!");
    lcd.clear();
    lcd.print("ERRO MQTT!");
    lcd.setCursor(0, 1);
    lcd.print("Verifique rede");
  }
}

// ==========================================
// CALLBACK MQTT
// ==========================================
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.println("\n========================================");
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.println("]");
  
  // Converter payload para string
  String payloadStr = "";
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  
  Serial.print("Payload: ");
  Serial.println(payloadStr);
  
  // Parse JSON
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, payloadStr);
  
  if (error) {
    Serial.print("ERRO ao parsear JSON: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Processar alerta
  processAlert(doc);
  
  Serial.println("========================================\n");
}

// ==========================================
// PROCESSAR ALERTA
// ==========================================
void processAlert(JsonDocument& doc) {
  // Extrair dados do JSON
  int riskScore = doc["risk"]["score"] | 0;
  String riskLevel = doc["risk"]["level"] | "BAIXO";
  String state = doc["location"]["abbr"] | "--";
  String city = doc["location"]["city"] | "N/A";
  
  // Atualizar variáveis globais
  currentRiskScore = riskScore;
  currentRiskLevel = riskLevel;
  currentState = state;
  
  Serial.println("--- DADOS DO ALERTA ---");
  Serial.print("Risco: ");
  Serial.print(riskLevel);
  Serial.print(" (");
  Serial.print(riskScore);
  Serial.println("/100)");
  Serial.print("Estado: ");
  Serial.print(state);
  Serial.print(" (");
  Serial.print(city);
  Serial.println(")");
  Serial.println("-----------------------");
  
  // Atualizar LEDs
  updateLEDs(riskScore);
  
  // Atualizar LCD
  updateLCD(riskLevel, riskScore, state);
  
  // Ativar buzzer se necessário
  activateBuzzer(riskScore);
  
  // Ativar laser em situação crítica
  if (riskScore >= 70) {
    digitalWrite(LASER_PIN, HIGH);
    Serial.println("🔴 LASER ATIVADO!");
  } else {
    digitalWrite(LASER_PIN, LOW);
  }
}

// ==========================================
// ATUALIZAR LEDs
// ==========================================
void updateLEDs(int riskScore) {
  // Desligar todos
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_ORANGE_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  
  // Ligar LED correspondente ao nível de risco
  if (riskScore < 30) {
    digitalWrite(LED_GREEN_PIN, HIGH);
    Serial.println("LED: VERDE (Risco BAIXO)");
  } else if (riskScore < 50) {
    digitalWrite(LED_YELLOW_PIN, HIGH);
    Serial.println("LED: AMARELO (Risco MÉDIO)");
  } else if (riskScore < 70) {
    digitalWrite(LED_ORANGE_PIN, HIGH);
    Serial.println("LED: LARANJA (Risco ALTO)");
  } else {
    digitalWrite(LED_RED_PIN, HIGH);
    Serial.println("LED: VERMELHO (Risco CRÍTICO)");
  }
}

// ==========================================
// ATUALIZAR LCD
// ==========================================
void updateLCD(String riskLevel, int riskScore, String state) {
  lcd.clear();
  
  // Linha 1: Nível de risco
  lcd.setCursor(0, 0);
  lcd.print("Risco: ");
  lcd.print(riskLevel);
  
  // Linha 2: Score e Estado
  lcd.setCursor(0, 1);
  lcd.print(riskScore);
  lcd.print("/100 ");
  lcd.print(state);
  
  Serial.println("LCD atualizado");
}

// ==========================================
// ATIVAR BUZZER
// ==========================================
void activateBuzzer(int riskScore) {
  // Buzzer só ativa para risco ALTO ou CRÍTICO
  if (riskScore >= 50) {
    int frequency = (riskScore >= 70) ? 2000 : 1000; // 2kHz para crítico, 1kHz para alto
    
    tone(BUZZER_PIN, frequency);
    buzzerActive = true;
    buzzerStartTime = millis();
    
    Serial.print("BUZZER ATIVADO (");
    Serial.print(frequency);
    Serial.println(" Hz por 10s)");
  } else {
    noTone(BUZZER_PIN);
    buzzerActive = false;
  }
}

// ==========================================
// DESATIVAR TODOS ALERTAS
// ==========================================
void deactivateAllAlerts() {
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_ORANGE_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LASER_PIN, LOW);
  noTone(BUZZER_PIN);
  buzzerActive = false;
}
