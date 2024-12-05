void CarregaDadosSD (fs::FS &fs, const char *path)
{
  Serial.printf("Lendo arquivo: %s\n", path);

  File file = fs.open(path);
  if (!file) 
  {
    Serial.println("Falha ao ler o arquivo, provavelmente o arquivo não existe");

    Serial.printf("Criando arquivo: %s\n", path);
    File file = fs.open(path, FILE_WRITE);
    if (!file) 
    {
      Serial.println("Falha ao escrever, provavelmente o disco esta protegito para escrita");
      return;
    }
    file.close();
    GravaDadosSD(fs, path);
  }
  else
  {
    // le dados
  }
}

void GravaDadosSD (fs::FS &fs, const char *path)
{
  File file = fs.open(path, FILE_WRITE);
  if (!file) 
  {
    Serial.println("Falha ao escrever, provavelmente o disco esta protegito para escrita");
    return;
  }
  
}


void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) 
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

