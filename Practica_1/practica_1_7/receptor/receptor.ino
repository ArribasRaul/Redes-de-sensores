#include <Wire.h>
float fnum;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  I2C_readAnything (fnum);
  if (fnum > 9000000.0){
    Serial.println("");
    Serial.println("Acelerometro:");
  }
  else if (fnum > 8000000.0){
    Serial.println("");
    Serial.println("Giroscopio:");
  }
  else if (fnum > 7000000.0){
    Serial.println("");
    Serial.println("Sensor magnetico:");
  }
  else{
    Serial.println(fnum);
  }
}

int I2C_readAnything(float& value)
  {
    byte * p = (byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++)
          *p++ = Wire.read();
    return i;
  }  // end of I2C_readAnything