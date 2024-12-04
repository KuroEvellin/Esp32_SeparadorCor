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

  bool movePosicaoInicial = false;
  bool movePosicaoColetor = false;
  bool calculaCor = false;
  bool abrePorta = false;
  bool fechaPorta = false;
};

AutoDB Auto;
 
void Automatico() 
{
  switch (Auto.AutoG7)
  {
    case MovePosicaoInicial:
    {
      Auto.movePosicaoInicial = true;
      if (movimentoConcluido)
      {
        Auto.movePosicaoInicial = false;
        movimentoConcluido = false;
        Auto.AutoG7 = AguardaPresencaPeca;
      }
      break;
    }

    case AguardaPresencaPeca:
    {
      if (SensorCor.corAtual.numeroColetor != 0)
      {
        Auto.AutoG7 = MovimentaSeparadores;
      }
      break;
    }

    case MovimentaSeparadores:
    {
      Auto.posicaoColetor = SensorCor.corAtual.numeroColetor;
      Auto.movePosicaoColetor = true;
      if (movimentoConcluido)
      {
        Auto.movePosicaoColetor = false;
        movimentoConcluido = false;
        Auto.AutoG7 = AbrePorta;
      }
      break;
    }

    case AbrePorta:
    {
      Auto.abrePorta = true;
      if (movimentoConcluido)
      {
        Auto.abrePorta = false;
        movimentoConcluido = false;
        Auto.AutoG7 = AguardaColetor;
      }
      break;
    }

    case AguardaColetor:
    {
      
      break;
    }
  }

}