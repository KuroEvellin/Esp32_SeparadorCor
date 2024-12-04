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
}