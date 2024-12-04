#include "Adafruit_TCS34725.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define QUANTIDADE_CORES 8

struct Cor // Declara estrutura para as cores
{
  float r, g, b = -1;
  int numeroColetor = 1;
};

struct Parametros // Lista de parametros da máquina, utilizado para gravar nos arquivos persistentes e posterior troca com app
{
  int PosicaoServoPorta[2] = {0, 90};
  int PosicaoServoDirecionadorED[2] = {85, 115};
  int PosicaoServoDirecionador12[2] = {85, 115};
  int PosicaoServoDirecionador34[2] = {85, 115};

  Cor cores[QUANTIDADE_CORES];
};

struct SensorCorDB // Declaração de dados globais referente ao sensor de cor
{
  Cor corAtual;
};

SensorCorDB SensorCor; // Instancia dos dados do sensor de cor

bool movimentoConcluido = false; // Variavel global para controle de movimento dos servos

Servo porta, direcionadorED, direcionador12, direcionador34; // Declaração dos servos
Adafruit_TCS34725 sensTCS = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_300MS, TCS34725_GAIN_1X); // Declaração do sensor de cor
LiquidCrystal_I2C lcd(0x27,16,2);  // Declaração do display LCD
Parametros parametros; // Declaração do banco de parâmetros

void setup() 
{
  Init();
}
void loop() 
{
  SensorCorMain();
  Automatico();
  ServosMain();

  /*
  lcd.clear();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Vermelho");
  */
}