<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=200&section=header&text=Setup%20Guide%20&fontSize=50&fontColor=fff&animation=twinkling&fontAlignY=40&desc=IoT%20Fire%20Prevention%20System&descAlignY=60&descSize=20">

## 🔧 Setup Guide | Guia de Configuração

### 📋 Index | Índice

<div align="left">

1. [Overview | Visão Geral](#-overview--visão-geral)
2. [Prerequisites | Pré-requisitos](#-prerequisites--pré-requisitos)
3. [InfluxDB Cloud Setup | Configuração do InfluxDB Cloud](#1️⃣-influxdb-cloud)
4. [Grafana Cloud Setup | Configuração do Grafana Cloud](#2️⃣-grafana-cloud)
5. [Node-RED Setup | Configuração do Node-RED](#3️⃣-node-red)
6. [Wokwi (ESP32) Setup | Configuração do Wokwi (ESP32)](#4️⃣-wokwi-esp32)
7. [WhatsApp Setup | Configuração do WhatsApp](#5️⃣-whatsapp)
8. [Testing the System | Testando o Sistema](#-testing-the-system--testando-o-sistema)
9. [Troubleshooting](#-troubleshooting)

</div>

### 🎯 Overview | Visão Geral

<div align="left">

**EN:** This system monitors **3 Brazilian states** at high risk of wildfires, using a distributed architecture for real-time data collection, processing, and visualization.
<br>
**PT-BR:** Este sistema monitora **3 estados brasileiros** com alto risco de queimadas, utilizando uma arquitetura distribuída para coleta, processamento e visualização de dados em tempo real.

- 🌳 **Amazonas** (Manaus)
- 🌾 **Mato Grosso do Sul** (Campo Grande)
- ⛰️ **Minas Gerais** (Belo Horizonte)

</div>

#### System Architecture | Arquitetura do Sistema
```
┌─────────────┐      ┌──────────────────┐      ┌─────────────┐
│   Wokwi     │      │     Node-RED     │      │  InfluxDB   │
│  (ESP32)    │──MQTT▶│  + OpenWeather   │──HTTP▶│    Cloud    │
└─────────────┘      └────────┬─────────┘      └──────┬──────┘
                              │                        │ (Flux)
                              │ (API)                  │
                              ▼                        ▼
                       ┌──────────────┐         ┌─────────────┐
                       │   WhatsApp   │         │   Grafana   │
                       │   (Alerts)   │         │    Cloud    │
                       └──────────────┘         └─────────────┘
```

### ✅ Prerequisites | Pré-requisitos

<div align="left">

**EN:**
- [ ] **Node.js and npm**: Essential for installing and running Node-RED.
- [ ] **InfluxDB Cloud Account**: Where time-series data will be stored. [Create a free account](https://cloud2.influxdata.com/signup).
- [ ] **Grafana Cloud Account**: To create visualization dashboards. [Create a free account](https://grafana.com/auth/sign-up/create-user).
- [ ] **OpenWeatherMap API Key**: To get real weather data. [Get a free API Key](https://openweathermap.org/api).
- [ ] **Wokwi Account**: To simulate the ESP32 hardware. [Access Wokwi](https://wokwi.com/).
- [ ] **WhatsApp API**: For sending alerts (e.g., CallMeBot or Twilio).

**PT-BR:**
- [ ] **Node.js e npm**: Essencial para instalar e executar o Node-RED.
- [ ] **Conta no InfluxDB Cloud**: Onde os dados de séries temporais serão armazenados. [Criar conta gratuita](https://cloud2.influxdata.com/signup).
- [ ] **Conta no Grafana Cloud**: Para criar os dashboards de visualização. [Criar conta gratuita](https://grafana.com/auth/sign-up/create-user).
- [ ] **API Key do OpenWeatherMap**: Para obter dados meteorológicos reais. [Obter API Key gratuita](https://openweathermap.org/api).
- [ ] **Conta no Wokwi**: Para simular o hardware ESP32. [Acessar Wokwi](https://wokwi.com/).
- [ ] **API do WhatsApp**: Para o envio de alertas (ex: CallMeBot ou Twilio).

</div>

### 1️⃣ InfluxDB Cloud

<div align="left">

#### EN

##### 1.1 Create Account and Bucket
1. **Create a free account** on [InfluxDB Cloud](https://cloud2.influxdata.com/signup), selecting your preferred region (e.g., `us-east-1`).
2. In the side menu, navigate to **Load Data > Buckets**.
3. Click **Create Bucket** and configure:
   - **Name**: `fire-detection`
   - **Delete Data**: `Never` (or set a retention policy, e.g., 30 days).
4. Click **Create**.

##### 1.2 Generate API Token
1. Go to **Load Data > API Tokens**.
2. Click **Generate API Token > All Access API Token**.
3. **Copy the generated token and store it in a safe place.** It will be used in Node-RED and Grafana.

##### 1.3 Get Organization Info
1. In the top-left menu (profile icon), note your **Organization Name** (Org).
2. The **URL** will be the same one you use to access InfluxDB Cloud (e.g., `https://us-east-1-1.aws.cloud2.influxdata.com`).

#### PT-BR

##### 1.1 Criar Conta e Bucket
1. **Crie uma conta gratuita** no [InfluxDB Cloud](https://cloud2.influxdata.com/signup), selecionando a região de sua preferência (ex: `us-east-1`).
2. No menu lateral, navegue até **Load Data > Buckets**.
3. Clique em **Create Bucket** e configure:
   - **Name**: `fire-detection`
   - **Delete Data**: `Never` (ou defina uma política de retenção, ex: 30 dias).
4. Clique em **Create**.

##### 1.2 Gerar Token de API
1. Vá para **Load Data > API Tokens**.
2. Clique em **Generate API Token > All Access API Token**.
3. **Copie o token gerado e guarde-o em local seguro.** Ele será usado no Node-RED e no Grafana.

##### 1.3 Obter Informações da Organização
1. No menu superior esquerdo (ícone do perfil), anote o **Nome da sua Organização** (Org).
2. A **URL** será a mesma que você usa para acessar o InfluxDB Cloud (ex: `https://us-east-1-1.aws.cloud2.influxdata.com`).

</div>

### 2️⃣ Grafana Cloud

<div align="left">

#### EN

##### 2.1 Create Account
1. Create a free account on [Grafana Cloud](https://grafana.com/auth/sign-up/create-user) and follow the onboarding process.

##### 2.2 Add Data Source (InfluxDB)
1. In the side menu, go to **Connections > Data Sources** and click **Add data source**.
2. Select **InfluxDB**.
3. Fill in the settings:
   - **Query Language**: `Flux`
   - **URL**: Your InfluxDB Cloud URL.
   - **Custom HTTP Headers**:
     - **Header**: `Authorization`
     - **Value**: `Token YOUR_INFLUXDB_TOKEN` (replace with your copied token).
   - **InfluxDB Details**:
     - **Organization**: Your InfluxDB organization name.
     - **Token**: `YOUR_INFLUXDB_TOKEN` (the same token again).
     - **Default Bucket**: `fire-detection`.
4. Click **Save & Test**. A success message should appear.

#### PT-BR

##### 2.1 Criar Conta
1. Crie uma conta gratuita no [Grafana Cloud](https://grafana.com/auth/sign-up/create-user) e siga o processo de onboarding.

##### 2.2 Adicionar Data Source (InfluxDB)
1. No menu lateral, vá em **Connections > Data Sources** e clique em **Add data source**.
2. Selecione **InfluxDB**.
3. Preencha as configurações:
   - **Query Language**: `Flux`
   - **URL**: Sua URL do InfluxDB Cloud.
   - **Custom HTTP Headers**:
     - **Header**: `Authorization`
     - **Value**: `Token SEU_TOKEN_DO_INFLUXDB` (substitua pelo token que você copiou).
   - **InfluxDB Details**:
     - **Organization**: O nome da sua organização no InfluxDB.
     - **Token**: `SEU_TOKEN_DO_INFLUXDB` (o mesmo token novamente).
     - **Default Bucket**: `fire-detection`.
4. Clique em **Save & Test**. Uma mensagem de sucesso deve aparecer.

</div>

### 3️⃣ Node-RED

<div align="left">

#### EN

##### 3.1 Install Node-RED and Dependencies
```bash
# Install Node-RED globally
npm install -g --unsafe-perm node-red

# Navigate to the Node-RED user directory
cd ~/.node-red

# Install required nodes
npm install node-red-node-openweathermap node-red-contrib-influxdb node-red-contrib-whatsapp-cmb

# Start Node-RED
node-red
```
Access the interface at `http://localhost:1880`.

##### 3.2 Import the Flow
1. Copy the entire content of the `flows.json` file from the repository.
2. In Node-RED, go to **Menu (☰) > Import**, paste the JSON, and click **Import**.

##### 3.3 Configure the Nodes
1. **InfluxDB:** Double-click the `☁️ InfluxDB Cloud` node, click the pencil icon to edit the server, and fill in your InfluxDB **URL**, **Token**, and **Organization**.
2. **OpenWeather:** Get your API Key from the [OpenWeather website](https://openweathermap.org/api) and insert it into each of the three weather nodes (`🌳 Amazonas`, `🌾 MS`, `⛰️ MG`).
3. **WhatsApp:** Configure the `📱 WhatsApp Alert` node with your API Key and phone number.

##### 3.4 Deploy
Click the **Deploy** button in the top-right corner to save and activate the changes.

#### PT-BR

##### 3.1 Instalar Node-RED e Dependências
```bash
# Instalar Node-RED globalmente
npm install -g --unsafe-perm node-red

# Navegar para o diretório de usuário do Node-RED
cd ~/.node-red

# Instalar os nós necessários
npm install node-red-node-openweathermap node-red-contrib-influxdb node-red-contrib-whatsapp-cmb

# Iniciar o Node-RED
node-red
```
Acesse a interface em `http://localhost:1880`.

##### 3.2 Importar o Flow
1. Copie todo o conteúdo do arquivo `flows.json` do repositório.
2. No Node-RED, vá em **Menu (☰) > Import**, cole o JSON e clique em **Import**.

##### 3.3 Configurar os Nós
1. **InfluxDB:** Dê um duplo clique no nó `☁️ InfluxDB Cloud`, clique no ícone de lápis para editar o servidor e preencha a **URL**, **Token** e **Organização** com seus dados do InfluxDB.
2. **OpenWeather:** Obtenha sua API Key no [site do OpenWeather](https://openweathermap.org/api) e insira-a em cada um dos três nós de clima (`🌳 Amazonas`, `🌾 MS`, `⛰️ MG`).
3. **WhatsApp:** Configure o nó `📱 WhatsApp Alert` com a API Key e o número de telefone.

##### 3.4 Deploy
Clique no botão **Deploy** no canto superior direito para salvar e ativar as mudanças.

</div>

### 4️⃣ Wokwi (ESP32)

<div align="left">

#### EN

##### 4.1 Create and Configure the Project
1. Create a new ESP32 project on [Wokwi](https://wokwi.com/projects/new/esp32).
2. Add three tabs to the editor: `sketch.ino`, `diagram.json`, and `libraries.txt`.
3. Copy the content from the corresponding files in the `wokwi/` directory of the repository into these tabs.
4. The code is pre-configured to use the `Wokwi-GUEST` network and the public HiveMQ broker.

##### 4.2 Run the Simulation
1. Click **Start Simulation**.
2. In the **Serial Monitor**, you will see data being generated and published via MQTT.
3. You can interact with the sensors to simulate different scenarios.

#### PT-BR

##### 4.1 Criar e Configurar o Projeto
1. Crie um novo projeto ESP32 no [Wokwi](https://wokwi.com/projects/new/esp32).
2. Adicione três abas no editor: `sketch.ino`, `diagram.json` e `libraries.txt`.
3. Copie o conteúdo dos arquivos correspondentes do diretório `wokwi/` do repositório para essas abas.
4. O código já está configurado para usar a rede `Wokwi-GUEST` e o broker público do HiveMQ.

##### 4.2 Executar a Simulação
1. Clique em **Start Simulation**.
2. No **Serial Monitor**, você verá os dados sendo gerados e publicados via MQTT.
3. Você pode interagir com os sensores para simular diferentes cenários.

</div>

### 5️⃣ WhatsApp

<div align="left">

#### EN

##### Recommended Option: CallMeBot (Free)
1. Add the CallMeBot number to your contacts: **+34 644 34 87 60**.
2. Send this contact the following message: `I allow callmebot to send me messages`.
3. You will receive an **API Key** in response.
4. In Node-RED, configure the WhatsApp node with your international phone number and the received API Key.

#### PT-BR

##### Opção Recomendada: CallMeBot (Gratuito)
1. Adicione o número do CallMeBot aos seus contatos: **+34 644 34 87 60**.
2. Envie a seguinte mensagem para este contato: `I allow callmebot to send me messages`.
3. Você receberá uma **API Key** em resposta.
4. No Node-RED, configure o nó do WhatsApp com seu número de celular no formato internacional e a chave recebida.

</div>

### ✅ Testing the System | Testando o Sistema

<div align="left">

**EN:**
1. **Manual Simulation:** In Node-RED, use the `Inject` nodes to trigger manual simulations and check the `Debug` panel.
2. **Check InfluxDB:** In the InfluxDB **Data Explorer**, run a simple query to confirm data is arriving.
3. **Check Grafana:** Open your Grafana dashboard to see the panels updating in real-time.
4. **WhatsApp Alert:** Trigger a critical risk test. You should receive a message within seconds.

**PT-BR:**
1. **Simulação Manual:** No Node-RED, use os nós `Inject` para disparar simulações manuais e observe os dados no painel `Debug`.
2. **Verificar InfluxDB:** No **Data Explorer** do InfluxDB, execute uma query simples para confirmar que os dados estão chegando.
3. **Verificar Grafana:** Abra seu dashboard no Grafana e veja os painéis sendo atualizados em tempo real.
4. **Alerta WhatsApp:** Dispare um teste de risco crítico. Você deve receber uma mensagem no WhatsApp em poucos segundos.

</div>

### 🐛 Troubleshooting

<div align="left">

**EN:**
- **No data in InfluxDB?** Check that the Token, URL, and Org are correct in Node-RED. Check the Node-RED debug panel for connection errors.
- **OpenWeather not working?** Confirm the API Key is correct and active. Remember there's a call limit on the free plan.
- **Wokwi not connecting to MQTT?** Ensure the simulation is using the `Wokwi-GUEST` network (no password).

**PT-BR:**
- **Dados não chegam no InfluxDB?** Verifique se o Token, a URL e a Org estão corretos no Node-RED. Confira o painel de Debug para ver se há erros de conexão.
- **OpenWeather não funciona?** Confirme se a API Key está correta e ativa. Lembre-se que há um limite de chamadas na conta gratuita.
- **Wokwi não conecta ao MQTT?** Certifique-se de que a simulação está usando a rede `Wokwi-GUEST` (sem senha).

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=FF6B35&height=120&section=footer"/>

</div>