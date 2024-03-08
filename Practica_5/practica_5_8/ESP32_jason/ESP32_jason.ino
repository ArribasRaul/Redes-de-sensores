#include <ArduinoJson.h>

// Inicialización de variables y pines
const unsigned long interval = 10000; // 10000 milisegundos
unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    generateAndSendData();
  }
}

void generateAndSendData() {
  StaticJsonDocument<200> doc;
  doc["bv"] = random(0,100);
  doc["bu"] = "degC";
  doc["bt"] = millis() / 1000;

  String output;
  serializeJson(doc, output);
  Serial.println("JSON generado:");
  Serial.println(output);  
}
