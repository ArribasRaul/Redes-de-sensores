#include <EspMQTTClient.h>

#define IO_USERNAME  ""
#define IO_KEY       ""
#define WIFI_SSID    "Phone_1_3262"
#define WIFI_PASS    "raulhermozo"

EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASS,
  "io.adafruit.com",  // MQTT Broker server ip
  IO_USERNAME,
  IO_KEY,
  "TestClient",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);


void onConnectionEstablished() {
  Serial.println("Conexión establecida");
  
  client.subscribe("Rarribas/feeds/practica", [](const String &feed, const String &value) {});
}

void setup() {
  Serial.begin(115200);

  // Configuración de la conexión MQTT
  client.enableDebuggingMessages();
}

void loop() {
  // Permite que la librería maneje eventos MQTT
  client.loop();
}