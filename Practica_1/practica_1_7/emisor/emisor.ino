#include <Arduino_LSM9DS1.h>
#include "BBTimer.hpp"
#include <Wire.h>

int flag = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;
int contador = 0;

String inChar;

BBTimer my_t0(BB_TIMER0);

void t0Callback(){
  flag4 = 1;  
}

void setup() {
  Serial.begin(9600);
  my_t0.setupTimer(200000, t0Callback);
  my_t0.timerStart();

  while (!Serial);
  Serial.println("Started");
  Wire.begin(); // join i2c bus (address optional for writer)

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
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

  if (Serial.available()>0){
    inChar = Serial.readString();
    inChar.trim();
    Serial.println(inChar);
    
    if (inChar.equals("Envia")){
      flag2 = 1;
    }
  }

  if (flag3 == 1){
    flag3 = 0;
    Wire.beginTransmission(8); // transmit to device #8
    float sep = 9999999;
    I2C_writeAnything (sep);
    Wire.endTransmission();    // stop transmitting
    Serial.println("");
    Serial.println("Acelerometro:");
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (x1);
    Wire.endTransmission();    // stop transmitting
    Serial.println(x1);
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (y1);
    Wire.endTransmission();    // stop transmitting
    Serial.println(y1);
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (z1);
    Wire.endTransmission();    // stop transmitting
    Serial.println(z1);
    delay(200);

    Wire.beginTransmission(8); // transmit to device #8
    sep = 8888888;
    I2C_writeAnything (sep);
    Wire.endTransmission();    // stop transmitting
    Serial.println("");
    Serial.println("Giroscopio:");
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (x2);
    Wire.endTransmission();    // stop transmitting
    Serial.println(x2);
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (y2);
    Wire.endTransmission();    // stop transmitting
    Serial.println(y2);
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (z1);
    Wire.endTransmission();    // stop transmitting
    Serial.println(z1);
    delay(200);

    Wire.beginTransmission(8); // transmit to device #8
    sep = 7777777;
    I2C_writeAnything (sep);
    Wire.endTransmission();    // stop transmitting
    Serial.println("");
    Serial.println("Sensor magnetico:");
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (x3);
    Wire.endTransmission();    // stop transmitting
    Serial.println(x3);
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (y3);
    Wire.endTransmission();    // stop transmitting
    Serial.println(y3);
    delay(200);
    Wire.beginTransmission(8); // transmit to device #8
    I2C_writeAnything (z3);    
    Wire.endTransmission();    // stop transmitting
    Serial.println(z3);
    delay(200);
  }

  if (flag4 == 1){
    flag4 = 0;
    if  (flag2 == 1){
      contador++;
      flag = 1;
            
      if (contador >= 5){
        contador = 0;
        flag2 = 0;
        flag3 = 1;
      }
    }
  }
}

int I2C_writeAnything (const float& value)
  {
    const byte * p = (const byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++)
          Wire.write(*p++);
    return i;
  }  // end of I2C_writeAnything
