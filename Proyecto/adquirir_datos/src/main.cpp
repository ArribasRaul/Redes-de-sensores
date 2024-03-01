#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include "BBTimer.hpp"

int flag = 1;
int running = 0;

BBTimer tim0(BB_TIMER0);

void timCallback();

void timCallback()
{
  flag = 1;
}

void setup()
{
  IMU.begin();
  Serial.begin(9600);
  tim0.setupTimer(50000, timCallback);
  tim0.timerStart();
  Serial.println("------");
}

void loop()
{
  String s;
  if (Serial.available() > 0)
  {
    s = Serial.readString();
    s.trim();
    if (s.equals("start"))
      running = 1;
    else if (s.equals("stop"))
      running = 0;
  }
  
  if (running == 0)
    return;
  
  float x, y, z;
  if (flag == 1)
  {
    flag = 0;
    char buff[50];
    if (IMU.accelerationAvailable())
    {
      IMU.readAcceleration(x, y, z);
      sprintf(buff, "%.5f;%.5f;%.5f", x, z, y);
      Serial.println(buff);
    }
    if (IMU.gyroscopeAvailable())
    {
      IMU.readGyroscope(x, y, z);
      sprintf(buff, "%.5f;%.5f;%.5f", -x, z, y);
      Serial.println(buff);
    }
  }
}