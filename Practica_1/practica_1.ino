#include "BBTimer.hpp"
int lectura = A0;
bool blue_on = false;
BBTimer my_t0(BB_TIMER0);

void t0Callback()
{
  blue_on = !blue_on;
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(lectura,INPUT); 
  //Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  my_t0.setupTimer(50000, t0Callback);
  my_t0.timerStart();
  }

void loop() {
  // put your main code here, to run repeatedly:
   digitalWrite(LED_BLUE, blue_on ? LOW : HIGH);
}
