struct DadosSD
{
  Parametros parametros;
  Dados dados;
};

/*
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
*/

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