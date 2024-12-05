void Automatico() 
{
  lcd.setCursor(0,0);
  switch (Auto.AutoG7)
  { 
    case MovePosicaoInicial:
    {
      lcd.print("Reposicionando...");
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
      lcd.print("Aguardando peca");
      if (SensorCor.corAtual.numeroColetor != 0)
      {
        Auto.AutoG7 = MovimentaSeparadores;
      }
      break;
    }

    case MovimentaSeparadores:
    {
      lcd.print("Movi. Separador...");
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
      lcd.print("Abrindo porta...");
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
      lcd.print("Aguarda coletor...");
      delay(2000);
      Auto.AutoG7 = ContabilizaPeca;
      break;
    }

    case ContabilizaPeca:
    {
      lcd.print("Contabiliza peca...");
      dados.PecasSeparadasPorCor[Auto.numeroCor]++;
      dados.PecasSeparadasPorColetor[Auto.posicaoColetor - 1]++;
      Auto.AutoG7 = FechaPorta;
      break;
    }

    case FechaPorta:
    {
      lcd.print("Fechando porta...");
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
      lcd.print("Gravando dados...");
      GravaDadosSD(SD, "/Teste");
      Auto.AutoG7 = AguardaPresencaPeca;
      break;
    }
  }
}