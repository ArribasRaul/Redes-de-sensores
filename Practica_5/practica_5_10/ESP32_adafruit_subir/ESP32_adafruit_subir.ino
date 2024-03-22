#include "AdafruitIO_WiFi.h"

#define IO_USERNAME  ""
#define IO_KEY       ""
#define WIFI_SSID    "Phone_1_3262"
#define WIFI_PASS    "raulhermozo"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

void setup() {
  Serial.begin(115200);

  while(! Serial);

  Serial.print("Conectando a Adafruit IO");
  io.connect();

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  // Tu código para obtener datos
  float valor = obtenerDatos();

  // Subir datos al feed
  io.feed("practica")->save(valor);
  Serial.println(valor);

  delay(5000);  // Esperar 5 segundos antes de enviar el siguiente dato
}

float obtenerDatos() {
  // Implementar lógica para obtener datos
  io.run();
  return random(0,35);
}