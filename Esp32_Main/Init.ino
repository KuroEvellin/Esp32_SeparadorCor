void Init()
{
  // Inicia o monitor serial
  Serial.begin(9600);
  delay(500);

  // Incia os servos
  InitServos();

  // Incia LCD
  lcd.init();

  // Inicia sensor de cor
  SensorCorInit();

  // Inicia Cartão SD
  if (!SD.begin()) 
  {
    Serial.println("Card Mount Failed");
    return;
  }
  // Verifica o tipo do cartão
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  // Verifica o tamanho do cartão
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  // Incializa Automático
  Auto.AutoG7 = MovePosicaoInicial;

  // Carrega dados salvos SD


  // Dados mocados para teste
  int i = 0;
  parametros.cores[i].r = 0.27;
  parametros.cores[i].g = 0.38;
  parametros.cores[i].b = 0.34;
  parametros.cores[i].numeroColetor = 0;
  i++;
  parametros.cores[i].r = 0.61; // vermelho
  parametros.cores[i].g = 0.21;
  parametros.cores[i].b = 0.17;
  parametros.cores[i].numeroColetor = 4;
  i++;
  parametros.cores[i].r = 0.26; // verde
  parametros.cores[i].g = 0.47;
  parametros.cores[i].b = 0.27;
  parametros.cores[i].numeroColetor = 3;
  i++;
  parametros.cores[i].r = 0.36; // roxo
  parametros.cores[i].g = 0.29;
  parametros.cores[i].b = 0.35;
  parametros.cores[i].numeroColetor = 2;
  i++;
  parametros.cores[i].r = 0.60; // laranja
  parametros.cores[i].g = 0.21;
  parametros.cores[i].b = 0.19;
  parametros.cores[i].numeroColetor = 1;
  i++;
  parametros.cores[i].r = 0.40; // amarelo
  parametros.cores[i].g = 0.43;
  parametros.cores[i].b = 0.17;
  parametros.cores[i].numeroColetor = 2;  i++;
  i++;
  parametros.cores[i].r = 0.22;
  parametros.cores[i].g = 0.36; // azul
  parametros.cores[i].b = 0.42;
  parametros.cores[i].numeroColetor = 4;

}