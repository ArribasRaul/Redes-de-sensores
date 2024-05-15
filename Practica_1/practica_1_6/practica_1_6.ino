#include <Arduino_LSM9DS1.h>
#include "BBTimer.hpp"

int flag = 0;
int flag2 = 0;
int contador = 0;

BBTimer my_t0(BB_TIMER0);

void t0Callback(){
  flag = 1;
  if (contador < 10){
    contador++;
  }
  else if (contador >= 10){
    contador = 0;
    flag2 = 1;
  }
}

void setup() {
  Serial.begin(9600);
  my_t0.setupTimer(100000, t0Callback);
  my_t0.timerStart();

  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in g's");
  Serial.println("X\tY\tZ");

  Serial.println();
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  Serial.println();
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");
}

void loop() {
  float x1, y1, z1;
  float x2, y2, z2;
  float x3, y3, z3;

  if (flag == 1){
    flag = 0;
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x1, y1, z1);
    }
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x2, y2, z2);
    }
    if (IMU.magneticFieldAvailable()) {
      IMU.readMagneticField(x3, y3, z3);
    }
  }

  if (flag2 == 1){
    flag2 = 0;
    Serial.println("Acelerometro:");
    Serial.print(x1);
    Serial.print('\t');
    Serial.print(y1);
    Serial.print('\t');
    Serial.println(z1);

    //Serial.println();
    Serial.println("Giroscopio:");
    Serial.print(x2);
    Serial.print('\t');
    Serial.print(y2);
    Serial.print('\t');
    Serial.println(z2);

    //Serial.println();
    Serial.println("Magnetómetro:");
    Serial.print(x3);
    Serial.print('\t');
    Serial.print(y3);
    Serial.print('\t');
    Serial.println(z3);
    Serial.println("----------------------");
  }
}
