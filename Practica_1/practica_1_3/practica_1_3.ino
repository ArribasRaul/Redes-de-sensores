#include "BBTimer.hpp"
int lectura = A0;
int val = 0;
int esc = 0;
int flag = 0;
int PWM = 11;
BBTimer my_t0(BB_TIMER0);

void t0Callback()
{
  flag = 1;
}

void setup() {
  pinMode(lectura,INPUT);
  pinMode(PWM,OUTPUT); 
  Serial.begin(9600);

  my_t0.setupTimer(200, t0Callback);
  my_t0.timerStart();
  }

void loop() {
  if (flag == 1 ){
    flag = 0;
    val = analogRead(lectura);
    esc = val/1024.0*255.0;
    analogWrite(PWM, esc);
   }
}
