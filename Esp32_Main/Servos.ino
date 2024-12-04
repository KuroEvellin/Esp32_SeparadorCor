void InitServos()
{
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  porta.setPeriodHertz(50);
  direcionadorED.setPeriodHertz(50);
  direcionador12.setPeriodHertz(50);
  direcionador34.setPeriodHertz(50);
  porta.attach(32, 0, 2500);
  direcionadorED.attach(33, 0, 2500);
  direcionador12.attach(13, 0, 2500);
  direcionador34.attach(14, 0, 2500);
}

void ServosMain()
{
  if (Auto.movePosicaoInicial)
    ServoPosicaoIncial();
  if (Auto.movePosicaoColetor)
    ServoMovimentaColetor(Auto.posicaoColetor);
  if (Auto.abrePorta)
    ServoAbrePorta();
  if (Auto.fechaPorta)
    ServoFechaPorta();
}

void ServoMovimentaColetor(int numeroColetor)
{
  if (numeroColetor <= 0 || numeroColetor >= 5)
    return;

  if (numeroColetor <= 2 )
  {
    direcionadorED.write(parametros.PosicaoServoDirecionadorED[0]);
    delay(500);
    if (numeroColetor == 1)
    {
      direcionador12.write(parametros.PosicaoServoDirecionador12[0]);
    }
    else
    {
      direcionador12.write(parametros.PosicaoServoDirecionador12[1]);
    }
  }
  else
  {
    direcionadorED.write(parametros.PosicaoServoDirecionadorED[1]);
    delay(500);
    if (numeroColetor == 3)
    {
      direcionador12.write(parametros.PosicaoServoDirecionador34[0]);
    }
    else
    {
      direcionador12.write(parametros.PosicaoServoDirecionador34[1]);
    }
  }
  delay(500);
  movimentoConcluido = true;
}

void ServoAbrePorta()
{
  porta.write(parametros.PosicaoServoPorta[0]);
  delay(500);
  movimentoConcluido = true;
}

void ServoFechaPorta()
{
  porta.write(parametros.PosicaoServoPorta[1]);
  delay(500);
  movimentoConcluido = true;
}

void ServoPosicaoIncial()
{
  ServoFechaPorta();
  ServoMovimentaColetor(1);
}