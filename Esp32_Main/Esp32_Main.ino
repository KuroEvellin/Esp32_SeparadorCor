#include "Adafruit_TCS34725.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

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
  //porta.write(90);
  //direcionadorED.write(90);
  //direcionador12.write(90);
  //direcionador34.write(90);

  lcd.clear();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print(Auto.AutoG7);
}