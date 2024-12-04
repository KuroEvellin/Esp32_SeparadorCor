

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

void Servos()
{

}