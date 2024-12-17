#include "Adafruit_TCS34725.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#define MQTT_VERSION MQTT_VERSION_3_1_1
#define QUANTIDADE_CORES 8

struct Cor // Declara estrutura para as cores
{
  float r, g, b = 0;
  int numeroColetor = 1;
};

struct Parametros // Lista de parametros da máquina, utilizado para gravar nos arquivos persistentes e posterior troca com app
{
  int PosicaoServoPorta[2] = {90, 0};
  int PosicaoServoDirecionadorED[2] = {72, 105};
  int PosicaoServoDirecionador12[2] = {70, 100};
  int PosicaoServoDirecionador34[2] = {80, 115};

  Cor cores[QUANTIDADE_CORES];

  // Configurações de WiFi
  char* ssid = "your_wifi_ssid";
  char* password = "your_wifi_password";

  // Configurações do Broker MQTT
  char* mqtt_server = "34bab4bb63014dce9e71f4ad8fb6ffc2.s2.eu.hivemq.cloud";
  char* mqtt_username = "your_mqtt_client_username";
  char* mqtt_password = "your_mqtt_client_password";
  int mqtt_port =8883;
};

struct Dados // Lista de dados, como contadores, utilizado para gravar nos arquivos persistentes e posterior troca com app
{
  int PecasSeparadasPorCor[QUANTIDADE_CORES];
  int PecasSeparadasPorColetor[4];
};

enum G7_Auto // Define etapas automático
{
  MovePosicaoInicial,
  AguardaPresencaPeca,
  MovimentaSeparadores,
  AbrePorta,
  AguardaColetor,
  ContabilizaPeca,
  FechaPorta,
  GravaLog
};

struct AutoDB // Define variaváveis do automático
{
  G7_Auto AutoG7;

  int posicaoColetor = 0;
  int numeroCor = 0;

  bool movePosicaoInicial = false;
  bool movePosicaoColetor = false;
  bool calculaCor = false;
  bool abrePorta = false;
  bool fechaPorta = false;
};

struct SensorCorDB // Declaração de dados globais referente ao sensor de cor
{
  Cor corAtual;
  int numeroCor;
};

AutoDB Auto; // Instancia de dados globais do Automático
SensorCorDB SensorCor; // Instancia dos dados globais do sensor de cor

bool movimentoConcluido = false; // Variavel global para controle de movimento dos servos

Servo porta, direcionadorED, direcionador12, direcionador34; // Declaração dos servos
Adafruit_TCS34725 sensTCS = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_300MS, TCS34725_GAIN_1X); // Declaração do sensor de cor
LiquidCrystal_I2C lcd(0x27,16,2);  // Declaração do display LCD

Parametros parametros; // Declaração do banco de parâmetros
Dados dados; // Declaração do banco de dados para estatistica

void setup() 
{
  Init();
}

void loop() 
{
  SensorCorMain();
  Automatico();
  ServosMain();

  lcd.setCursor(0,1);
  lcd.print("");
  lcd.print(dados.PecasSeparadasPorColetor[0]);
  lcd.print(" ");
  lcd.print(dados.PecasSeparadasPorColetor[1]);
  lcd.print(" ");
  lcd.print(dados.PecasSeparadasPorColetor[2]);
  lcd.print(" ");
  lcd.print(dados.PecasSeparadasPorColetor[3]);
  lcd.print(" ");
}