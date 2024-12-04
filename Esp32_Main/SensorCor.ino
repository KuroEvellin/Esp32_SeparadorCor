void SensorCorInit()
{
  // Inicia o sensor e diz se ele conectou ou não
  if (sensTCS.begin()) 
  {
    Serial.println("Sensor conectado");
  }
  else 
  { 
    // Se o sensor não conectou , mostra a mensagem de erro
    Serial.println("Sensor com problema");
  } 
}

void SensorCorMain()
{
  JulgaCor();
}

void JulgaCor()
{
  uint16_t r, g, b, c;
  int totalrgb;
  int CorMaisProxima = -1;
  float menorDistancia = 10; 
  float rn, gn, bn = -1;

  // Coleta rgb
  sensTCS.getRawData(&r, &g, &b, &c);

  // Normaliza proporsões rgb
  totalrgb = r + g + b;
  rn = (float)(r / totalrgb);
  gn = (float)(g / totalrgb);
  bn = (float)(b / totalrgb);

  // Procura cor mais proxima
  for (int i = 0; i < QUANTIDADE_CORES; i++)
  {
    if (parametros.cores[i].numeroColetor <= 0)
      continue;

    float distancia = sqrt(pow(rn - parametros.cores[i].r, 2) + pow(gn - parametros.cores[i].g, 2) + pow(bn - parametros.cores[i].b, 2));

    if (distancia < menorDistancia)
    {
      menorDistancia = distancia;
      CorMaisProxima = i;
    }
  }
 
  SensorCor.corAtual.r = rn;
  SensorCor.corAtual.g = gn;
  SensorCor.corAtual.b = bn;
  SensorCor.corAtual.numeroColetor = CorMaisProxima;
}