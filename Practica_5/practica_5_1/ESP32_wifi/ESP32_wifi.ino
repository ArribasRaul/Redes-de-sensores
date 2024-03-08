#include "WiFi.h"

const char* ssid = "WLAN_XD";

const char* password = "975230712";

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  //WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.println("Connecting to WiFi..");

  }

  Serial.println("Connected to the WiFi network");
  Serial.print("Tu IP es: ");
  Serial.println(WiFi.localIP());
}

void loop() {}