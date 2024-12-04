#include "Adafruit_TCS34725.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

struct Cor // Declara estrutura para as cores
{
  int r, g, b;
  int numeroColetor = 1;
};

struct Parametros // Lista de parametros da máquina, utilizado para gravar nos arquivos persistentes e posterior troca com app
{
  int PosicaoServoPorta[2] = {0, 90};
  int PosicaoServoDirecionadorED[2] = {85, 115};
  int PosicaoServoDirecionador12[2] = {85, 115};
  int PosicaoServoDirecionador34[2] = {85, 115};

  Cor cores[8];
};

Servo porta, direcionadorED, direcionador12, direcionador34;
Adafruit_TCS34725 sensTCS = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_300MS, TCS34725_GAIN_1X);
LiquidCrystal_I2C lcd(0x27,16,2);
Parametros parametros;

void setup() 
{
  Init();
}
void loop() 
{
  //declaramos as variáveis
  uint16_t r, g, b, c, TempCor, LUX;

  //O sensor lê os valores do Vermelho (R), Verde(G), Azul(B)
  sensTCS.getRawData(&r, &g, &b, &c);
  
  //Calcula a temperatura da cor
  TempCor = sensTCS.calculateColorTemperature(r, g, b);
  
  //Calcula a luminosidade 
  LUX = sensTCS.calculateLux(r, g, b);
  Serial.print("Vermelho(R): ");
  Serial.print(r); //escreve no monitor serial o valor do Vermelho
  Serial.print("   Verde(G): ");
  Serial.print(g);//escreve no monitor serial o valor do Verde
  Serial.print("   Azul(B): ");
  Serial.print(b);//escreve no monitor serial o valor do Azul
  Serial.print("   Temperatura: ");
  Serial.print(TempCor); //escreve no monitor serial o valor da Temperatura da Cor
  Serial.print("   Intensidade: ");
  Serial.println(LUX); //escreve no monitor serial o valor da intensidade luminosa
  lcd.clear();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);

  //escreve no display a cor referente a leitura do sensor
  if (c < 5000) 
  {
    if (r > b && r > g && LUX > 100) { // faz a comparação dos valores para ver se é vermelho
      Serial.println("Vermelho"); 
      lcd.print("Vermelho");
      porta.write(90);
      direcionadorED.write(90);
      direcionador12.write(90);
      direcionador34.write(90);
      delay(100);
    }
    else if (g > r && g > b && LUX > 100 ) 
    { // faz a comparação dos valores para ver se é verde
      Serial.println("Verde"); 
      lcd.print("Verde");
      porta.write(90);
      direcionadorED.write(90);
      direcionador12.write(90);
      direcionador34.write(90);
      delay(100);
    }
    else if (b > r && b > g && LUX > 100) 
    { // faz a comparação dos valores para ver se é azul
      Serial.println("Azul"); 
      lcd.print("Azul");
      porta.write(90);
      direcionadorED.write(90);
      direcionador12.write(90);
      direcionador34.write(90);
      delay(100);
    }
    else
    {
      lcd.print("Nada");
      porta.write(0);
      direcionadorED.write(85);
      direcionador12.write(90);
      direcionador34.write(90);
    }
    delay(500);
  }
}