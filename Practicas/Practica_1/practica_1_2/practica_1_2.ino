#include "BBTimer.hpp"
int lectura = A0;
int val = 0;
int flag = 0;
BBTimer my_t0(BB_TIMER0);

void t0Callback()
{
  flag = 1;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(lectura,INPUT);
  Serial.begin(9600);

  my_t0.setupTimer(10000000, t0Callback);
  my_t0.timerStart();
  }

void loop() {
  // put your main code here, to run repeatedly:
   if (flag == 1 ){
    flag = 0;
    val = analogRead(lectura);
    Serial.println(val);
   }
}
