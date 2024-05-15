#include <Arduino_LSM9DS1.h>
#include "BBTimer.hpp"

int flag = 0;

BBTimer my_t0(BB_TIMER0);

void t0Callback(){
  flag = 1;
}

void setup() {
  Serial.begin(9600);
  my_t0.setupTimer(50000, t0Callback);
  my_t0.timerStart();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

}

void loop() {
  float x1, y1, z1;

  if (flag == 1){
    flag = 0;
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x1, y1, z1);
    // if(IMU.gyroscopeAvailable()){
    //   IMU.readGyroscope(x1, y1, z1);

      Serial.print(x1);
      Serial.print(',');
      Serial.print(y1);
      Serial.print(',');
      Serial.println(z1);
    }
  }
}
