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
      Auto.numeroCor = SensorCor.numeroCor;
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
      delay(5000);
      Auto.AutoG7 = ContabilizaPeca;
      break;
    }

    case ContabilizaPeca:
    {
      dados.PecasSeparadasPorCor[Auto.numeroCor]++;
      dados.PecasSeparadasPorColetor[Auto.posicaoColetor]++;
      Auto.AutoG7 = FechaPorta;
      break;
    }

    case FechaPorta:
    {
      Auto.fechaPorta = true;
      if (movimentoConcluido)
      {
        Auto.fechaPorta = false;
        movimentoConcluido = false;
        Auto.AutoG7 = GravaLog;
      }
      break;
    }
    case GravaLog:
    {
      Auto.AutoG7 = AguardaPresencaPeca;
      break;
    }
  }

}