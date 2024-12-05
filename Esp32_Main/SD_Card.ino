struct DadosSD
{
  Parametros parametros;
  Dados dados;
};

DadosSD persistencia;

void CarregaPersistencia()
{
  persistencia.parametros = parametros;
  persistencia.dados = dados;
  Serial.println(sizeof(persistencia));
}

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
    int size = sizeof(persistencia);
    char buff[size];
    file.readBytes(buff, size);
    memcpy(&persistencia, buff, size);
    dados = persistencia.dados;
    parametros = persistencia.parametros;
  }
}

void GravaDadosSD (fs::FS &fs, const char *path)
{
  CarregaPersistencia();
  File file = fs.open(path, FILE_WRITE);
  if (!file) 
  {
    Serial.println("Falha ao escrever, provavelmente o disco esta protegito para escrita");
    return;
  }

  int size = sizeof(persistencia);
  unsigned char buff[size];
  memcpy(buff, &persistencia, size);

  if(file.write(buff, size))
    Serial.println("Dados gravados com sucesso");
  else
    Serial.println("Erro ao gravar os dados");
  file.close();
}