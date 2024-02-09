#include <Arduino_LSM9DS1.h>
#include "BBTimer.hpp"

int flag = 0;

BBTimer my_t0(BB_TIMER0);

void t0Callback(){
  flag = 1;  
}

void setup() {
  Serial.begin(9600);
  my_t0.setupTimer(2000, t0Callback);
  my_t0.timerStart();
}

void loop() {
  float x1, y1, z1;
  float x2, y2, z2;

  if (flag == 1){
    flag = 0;
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x1, y1, z1);
      Serial.println("--------");
      Serial.print(x1);
      
      Serial.print(y1);
      
      Serial.println(z1);
      
    }
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x2, y2, z2);
      Serial.print(x2);
      
      Serial.print(y2);
      
      Serial.println(z2);
    }
  }
}
