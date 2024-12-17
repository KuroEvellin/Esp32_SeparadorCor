int conectarWiFi() {
  int delayTentativa = 1000;
  int tentativas = 3;

  Serial.println();
  Serial.print("[WiFi] Conectando na rede WiFi: ");
  Serial.println(parametros.ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(parametros.ssid, parametros.password);

  while (true) {

    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] Rede não encontrada!"); break;
      case WL_CONNECT_FAILED: Serial.print("[WiFi] Falha na conexão WiFi!"); break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Conexão perdida"); break;
      case WL_DISCONNECTED: Serial.println("[WiFi] WiFi está desconectado"); break;
      case WL_CONNECTED:
        Serial.println("[WiFi] WiFi conectado!");
        Serial.print("[WiFi] Endereço IP: ");
        Serial.println(WiFi.localIP());
        return 1;
        break;
      default:
        Serial.print("[WiFi] Estado do WiFi: ");
        Serial.println(WiFi.status());
        break;
    }
    delay(delayTentativa);

    if (tentativas <= 0) {
      Serial.print("[WiFi] Falha ao conectar com o WiFi!");
      WiFi.disconnect();
      return 0;
    } else {
      tentativas--;
    }
  }
}

WiFiClientSecure espClient;
PubSubClient client(espClient);

int conectarMQTT() {
  int delayTentativa = 1000;
  int tentativas = 3;

  while (true) {
    Serial.print("[MQTT] Tentativa de conexão MQTT...");

    const char* clientId = "ClienteESP32";

    // Definição da Will (A Will poderá ser usada no aplicativo para detectar que a comunicação com ESP32 falhou)
    byte willQoS = 1;
    const char* willTopic = "conexaoESP";
    const char* willMessage = "Falha na Conexão";
    boolean willRetain = false;

    client.connect(clientId, parametros.mqtt_username, parametros.mqtt_password , willTopic, willQoS, willRetain, willMessage);
    int rc = client.state();

    switch (rc) {
      case -4: // MQTT_CONNECTION_TIMEOUT
      Serial.println("[MQTT] O servidor não respondeu dentro do tempo de keepalive."); break;
      case -3: // MQTT_CONNECTION_LOST
        Serial.println("[MQTT] A conexão de rede foi perdida."); break;
      case -2: // MQTT_CONNECT_FAILED
        Serial.println("[MQTT] Falha na conexão com o servidor."); break;
      case -1: // MQTT_DISCONNECTED
        Serial.println("[MQTT] O cliente está desconectado."); break;

      case 0: // MQTT_CONNECTED
        Serial.println("[MQTT] Conexão bem-sucedida com o servidor."); 
        return 1; break;

      case 1: // MQTT_CONNECT_BAD_PROTOCOL
        Serial.println("[MQTT] O servidor não suporta a versão solicitada do MQTT."); break;
      case 2: // MQTT_CONNECT_BAD_CLIENT_ID
        Serial.println("[MQTT] O servidor rejeitou o identificador do cliente."); break;
      case 3: // MQTT_CONNECT_UNAVAILABLE
        Serial.println("[MQTT] O servidor não pôde aceitar a conexão."); break;
      case 4: // MQTT_CONNECT_BAD_CREDENTIALS
        Serial.println("[MQTT] Usuário ou senha rejeitados."); break;
      case 5: // MQTT_CONNECT_UNAUTHORIZED
        Serial.println("[MQTT] O cliente não foi autorizado a se conectar."); break;
      default:
        Serial.print("[MQTT] Estado desconhecido do cliente MQTT: ");
        Serial.println(client.state()); break;
    }
    delay(delayTentativa);

    if (tentativas <= 0) {
      Serial.print("[MQTT] Falha ao conectar com o Broker!");
      WiFi.disconnect();
      return 0;
    } else {
      tentativas--;
    }
  }
}

void callback(const char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];

  Serial.println("Mensagem recebida ["+String(topic)+"]"+incommingMessage);
}

void publicarMensagem(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Mensagem publicada ["+String(topic)+"]: "+payload);
}