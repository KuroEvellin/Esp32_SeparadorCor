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
  float totalrgb;
  int corMaisProxima, posicaoColetor = -1;
  float menorDistancia = 10; 
  float rn, gn, bn = -1;

  // Coleta rgb
  sensTCS.getRawData(&r, &g, &b, &c);

  // Normaliza proporções rgb
  totalrgb = r + g + b;
  rn = (float)(r / totalrgb);
  gn = (float)(g / totalrgb);
  bn = (float)(b / totalrgb);

  // Procura cor mais proxima
  for (int i = 0; i < QUANTIDADE_CORES; i++)
  {
    float distancia = sqrt(pow(rn - parametros.cores[i].r, 2) + pow(gn - parametros.cores[i].g, 2) + pow(bn - parametros.cores[i].b, 2));
    if (distancia < menorDistancia)
    {
      menorDistancia = distancia;
      posicaoColetor = parametros.cores[i].numeroColetor;
      corMaisProxima = i;
    }
  }
 
  // Coleta dados
  SensorCor.corAtual.r = rn;
  SensorCor.corAtual.g = gn;
  SensorCor.corAtual.b = bn;
  SensorCor.corAtual.numeroColetor = posicaoColetor;
  SensorCor.numeroCor = corMaisProxima;
  Serial.print("R: ");
  Serial.print(rn);
  Serial.print(" G: ");
  Serial.print(gn);
  Serial.print(" B: ");
  Serial.print(bn);
  Serial.print(" Distancia: ");
  Serial.print(menorDistancia);  
  Serial.print(" Coletor: ");
  Serial.print(posicaoColetor);
  Serial.print(" Cor: ");
  Serial.println(corMaisProxima);

}