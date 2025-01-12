
int conectarWiFi() 
{
  int delayTentativa = 5000;
  int tentativas = 3;

  Serial.println();
  Serial.print("[WiFi] Conectando na rede WiFi: ");
  Serial.println(parametros.ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(parametros.ssid, parametros.password);

  while (true) 
  {
    switch (WiFi.status()) 
    {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] Rede não encontrada!"); 
        break;
      case WL_CONNECT_FAILED: Serial.print("[WiFi] Falha na conexão WiFi!");
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Conexão perdida");
        break;
      case WL_DISCONNECTED: Serial.println("[WiFi] WiFi está desconectado");
        break;
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

    if (tentativas <= 0) 
    {
      Serial.print("[WiFi] Falha ao conectar com o WiFi!");
      WiFi.disconnect();
      return 0;
    }
    else 
    {
      tentativas--;
    }
  }
}

int conectarMQTT() 
{
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

    if (tentativas <= 0) 
    {
      Serial.print("[MQTT] Falha ao conectar com o Broker!");
      WiFi.disconnect();
      return 0;
    } 
    else 
    {
      tentativas--;
    }
  }
}

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Tentado conectar ao Servidor MQTT");
    String clientId = "ClienteESP32";
    if (client.connect(clientId.c_str(), parametros.mqtt_username, parametros.mqtt_password)) 
    {
      client.setBufferSize(1024);
      Serial.println("Conectado");
      publicarMensagem("conexaoESP", "Conexão Iniciada", false);
      client.subscribe("parametros");
      client.subscribe("novosParametros");
      client.subscribe("novasEstatisticas");
      client.subscribe("conexaoESP");
    }
    else 
    {
      Serial.print("Falha ao Conectar, status = ");
      Serial.print(client.state());
      Serial.println("Tentando de novo em  5 segundos...");
      delay(5000);
    }
  }
}

void callback(const char* topic, byte* payload, unsigned int length)
{
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  Serial.println("Mensagem recebida ["+String(topic)+"] "+length+" bytes : "+incommingMessage);

  if (String(topic) == "parametros")
  {
    desmontaJsonParametros(incommingMessage);
    publicarMensagem("parametrosReceber", montaJsonParametros(), false);
  }
  else if (String(topic) == "novosParametros")
  {
    publicarMensagem("parametrosReceber", montaJsonParametros(), false); 
  }
  else if (String(topic) == "novasEstatisticas")
  {
    publicarMensagem("estatisticasReceber", montaJsonEstatisticas(), false);
  }
}

void publicarMensagem(const char* topic, String payload , boolean retained)
{
  if (client.publish(topic, payload.c_str(), retained))
      Serial.println("Mensagem publicada ["+String(topic)+"]: "+payload);
}

String montaJsonParametros()
{
  JsonDocument doc;
  String buff;

  doc["PosicaoServoPortaAnguloMinimo"] = parametros.PosicaoServoPorta[0];
  doc["PosicaoServoPortaAnguloMaximo"] = parametros.PosicaoServoPorta[1];
  doc["PosicaoServoDirecionadorEDAnguloMinimo"] = parametros.PosicaoServoDirecionadorED[0];
  doc["PosicaoServoDirecionadorEDAnguloMaximo"] = parametros.PosicaoServoDirecionadorED[1];
  doc["PosicaoServoDirecionador12AnguloMinimo"] = parametros.PosicaoServoDirecionador12[0];
  doc["PosicaoServoDirecionador12AnguloMaximo"] = parametros.PosicaoServoDirecionador12[1];
  doc["PosicaoServoDirecionador34AnguloMinimo"] = parametros.PosicaoServoDirecionador34[0];
  doc["PosicaoServoDirecionador34AnguloMaximo"] = parametros.PosicaoServoDirecionador34[1];
  doc["Cor1R"] = parametros.cores[1].r;
  doc["Cor1G"] = parametros.cores[1].g;
  doc["Cor1B"] = parametros.cores[1].b;
  doc["Cor1Coletor"] = parametros.cores[1].numeroColetor;
  doc["Cor2R"] = parametros.cores[2].r;
  doc["Cor2G"] = parametros.cores[2].g;
  doc["Cor2B"] = parametros.cores[2].b;
  doc["Cor2Coletor"] = parametros.cores[2].numeroColetor;
  doc["Cor3R"] = parametros.cores[3].r;
  doc["Cor3G"] = parametros.cores[3].g;
  doc["Cor3B"] = parametros.cores[3].b;
  doc["Cor3Coletor"] = parametros.cores[3].numeroColetor;
  doc["Cor4R"] = parametros.cores[4].r;
  doc["Cor4G"] = parametros.cores[4].g;
  doc["Cor4B"] = parametros.cores[4].b;
  doc["Cor4Coletor"] = parametros.cores[4].numeroColetor;
  doc["Cor5R"] = parametros.cores[5].r;
  doc["Cor5G"] = parametros.cores[5].g;
  doc["Cor5B"] = parametros.cores[5].b;
  doc["Cor5Coletor"] = parametros.cores[5].numeroColetor;
  doc["Cor6R"] = parametros.cores[6].r;
  doc["Cor6G"] = parametros.cores[6].g;
  doc["Cor6B"] = parametros.cores[6].b;
  doc["Cor6Coletor"] = parametros.cores[6].numeroColetor;
  doc["Cor7R"] = parametros.cores[7].r;
  doc["Cor7G"] = parametros.cores[7].g;
  doc["Cor7B"] = parametros.cores[7].b;
  doc["Cor7Coletor"] = parametros.cores[7].numeroColetor;

  serializeJson(doc, buff);
  return buff;
}

void desmontaJsonParametros(String json)
{
  JsonDocument doc;
  deserializeJson(doc, json);

  parametros.PosicaoServoPorta[0] = (int) doc["PosicaoServoPortaAnguloMinimo"];
  parametros.PosicaoServoPorta[1] = (int) doc["PosicaoServoPortaAnguloMaximo"];
  parametros.PosicaoServoDirecionadorED[0] = (int) doc["PosicaoServoDirecionadorEDAnguloMinimo"];
  parametros.PosicaoServoDirecionadorED[1] = (int) doc["PosicaoServoDirecionadorEDAnguloMaximo"];
  parametros.PosicaoServoDirecionador12[0] = (int) doc["PosicaoServoDirecionador12AnguloMinimo"];
  parametros.PosicaoServoDirecionador12[1] = (int) doc["PosicaoServoDirecionador12AnguloMaximo"];
  parametros.PosicaoServoDirecionador34[0] = (int) doc["PosicaoServoDirecionador34AnguloMinimo"];
  parametros.PosicaoServoDirecionador34[1] = (int) doc["PosicaoServoDirecionador34AnguloMaximo"];
  parametros.cores[1].r = (float) doc["Cor1R"];
  parametros.cores[1].g = (float) doc["Cor1G"];
  parametros.cores[1].b = (float) doc["Cor1B"];
  parametros.cores[2].r = (float) doc["Cor2R"];
  parametros.cores[2].g = (float) doc["Cor2G"];
  parametros.cores[2].b = (float) doc["Cor2B"];
  parametros.cores[3].r = (float) doc["Cor3R"];
  parametros.cores[3].g = (float) doc["Cor3G"];
  parametros.cores[3].b = (float) doc["Cor3B"];
  parametros.cores[4].r = (float) doc["Cor4R"];
  parametros.cores[4].g = (float) doc["Cor4G"];
  parametros.cores[4].b = (float) doc["Cor4B"];
  parametros.cores[5].r = (float) doc["Cor5R"];
  parametros.cores[5].g = (float) doc["Cor5G"];
  parametros.cores[5].b = (float) doc["Cor5B"];
  parametros.cores[6].r = (float) doc["Cor6R"];
  parametros.cores[6].g = (float) doc["Cor6G"];
  parametros.cores[6].b = (float) doc["Cor6B"];
  parametros.cores[7].r = (float) doc["Cor7R"];
  parametros.cores[7].g = (float) doc["Cor7G"];
  parametros.cores[7].b = (float) doc["Cor7B"];
  parametros.cores[1].numeroColetor = (int) doc["Cor1Coletor"];
  parametros.cores[2].numeroColetor = (int) doc["Cor2Coletor"];
  parametros.cores[3].numeroColetor = (int) doc["Cor3Coletor"];
  parametros.cores[4].numeroColetor = (int) doc["Cor4Coletor"];
  parametros.cores[5].numeroColetor = (int) doc["Cor5Coletor"];
  parametros.cores[6].numeroColetor = (int) doc["Cor6Coletor"];
  parametros.cores[7].numeroColetor = (int) doc["Cor7Coletor"];

  //GravaDadosSD(SD, "/Teste");
}

String montaJsonMonitoramento()
{
  JsonDocument doc;
  String buff;

  doc["EstadoAtual"] = Auto.AutoG7;
	doc["CorAtual"] = SensorCor.numeroCor;
	doc["SensorR"] = SensorCor.corAtual.r;
	doc["SensorG"] = SensorCor.corAtual.g;
	doc["SensorB"] = SensorCor.corAtual.b;
	doc["PortaAberta"] = portaAberta;
	doc["ColetorAtual"] = Auto.posicaoColetor;

  serializeJson(doc, buff);
  return buff;
}

String montaJsonEstatisticas()
{
  JsonDocument doc;
  String buff;

  doc["PecasSeparadasCor1"] = dados.PecasSeparadasPorCor[1];
  doc["PecasSeparadasCor2"] = dados.PecasSeparadasPorCor[2];
  doc["PecasSeparadasCor3"] = dados.PecasSeparadasPorCor[3];
  doc["PecasSeparadasCor4"] = dados.PecasSeparadasPorCor[4];
  doc["PecasSeparadasCor5"] = dados.PecasSeparadasPorCor[5];
  doc["PecasSeparadasCor6"] = dados.PecasSeparadasPorCor[6];
  doc["PecasSeparadasCor7"] = dados.PecasSeparadasPorCor[7];
  doc["PecasSeparadasColetor1"] = dados.PecasSeparadasPorColetor[0];
  doc["PecasSeparadasColetor2"] = dados.PecasSeparadasPorColetor[1];
  doc["PecasSeparadasColetor3"] = dados.PecasSeparadasPorColetor[2];
  doc["PecasSeparadasColetor4"] = dados.PecasSeparadasPorColetor[3];

  serializeJson(doc, buff);
  return buff;
}


