#include <ArduinoJson.h>

int interval = 0; // primera medida
int previousMillis = 0;
int temp = 0;
int temp_sig = 0;

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));
  temp = random(0,35);
}

void loop()
{
  int currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    temp_sig = random(1,34);
    if (temp > temp_sig){
      temp = temp - random(0,3);
    }
    else if (temp <= temp_sig){
      temp = temp + random(0,3);
    }
    previousMillis = currentMillis;
    generateAndSendData();
  }
  interval = 10000; // 10 segundos
}

void generateAndSendData() {
  StaticJsonDocument<200> doc;
  doc["Temperatura"] = temp;
  doc["Unidades"] = "ºC";
  doc["Tiempo"] = millis() / 1000;

  String output;
  serializeJson(doc, output);
  Serial.println("JSON generado:");
  Serial.println(output);
}
