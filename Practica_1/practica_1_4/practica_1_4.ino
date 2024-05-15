#include "BBTimer.hpp"
int lectura = A0;
int val = 0;
int esc = 0;
int flag = 0;
int PWM = 11;
String inChar;
int val2 = 0;
int tiempo = 0;
int contador = 0;
int encendido = 0;
BBTimer my_t0(BB_TIMER0);

void t0Callback()
{
  contador++;
  if (contador >= tiempo){
    flag = 1;
  }
}

void setup() {
  pinMode(lectura,INPUT);
  pinMode(PWM,OUTPUT); 
  Serial.begin(9600);

  my_t0.setupTimer(100000, t0Callback);
  my_t0.timerStart();
  }

void loop() {
  if (Serial.available()>0){
    inChar = Serial.readString();
    inChar.trim();
    Serial.println(inChar);

    if (inChar.equals("ADC")){
      encendido = 0;
      val = analogRead(lectura);
      Serial.println(val);
    }
    
    else if (inChar.charAt(0) == 'A'){
      if (inChar.charAt(1) == 'D'){
        if (inChar.charAt(2) == 'C'){
          if (inChar.charAt(3) == '('){
            tiempo = inChar.substring(4,5).toInt()*10;
             encendido = 1;
           }
        }
      }
    }

    else if (inChar.charAt(0) == 'P'){
      if (inChar.charAt(1) == 'W'){
        if (inChar.charAt(2) == 'M'){
          if (inChar.charAt(3) == '('){
            val2 = inChar.substring(4,5).toInt();
            encendido = 0;
            if (val2 == 0){
              val = 0;
              analogWrite(PWM, val);
            }
            if (val2 == 1){
              val = 255/9;
              analogWrite(PWM, val);
            }
            if (val2 == 2){
              val = 255/9*2;
              analogWrite(PWM, val);
            }
            if (val2 == 3){
              val = 255/9*3;
              analogWrite(PWM, val);
            }
            if (val2 == 4){
              val = 255/9*4;
              analogWrite(PWM, val);
            }
            if (val2 == 5){
              val = 255/9*5;
              analogWrite(PWM, val);
            }
            if (val2 == 6){
              val = 255/9*6;
              analogWrite(PWM, val);
            }
            if (val2 == 7){
              val = 255/9*7;
              analogWrite(PWM, val);
            }
            if (val2 == 8){
              val = 255/9*8;
              analogWrite(PWM, val);
            }
            if (val2 == 9){
              val = 255/9*9;
              analogWrite(PWM, val);
            }
          }
        }
      }
    }
  }
  if (encendido == 1){
    if (flag == 1 and tiempo != 0){
      flag = 0;
      contador = 0;
      val = analogRead(lectura);
      Serial.println(val);
    }
    else if (flag == 1 and tiempo == 0){
      
    }
  }
}
