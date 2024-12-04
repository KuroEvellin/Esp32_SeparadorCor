enum G7_Auto // Define etapas automático
{
  MovePosicaoInicial,
  AguardaPresencaPeca,
  CalculaCor,
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
      if (false)
      {
        Auto.Autog7 = AguardaPresencaPeca;
      }
      break;
    }
  }

}