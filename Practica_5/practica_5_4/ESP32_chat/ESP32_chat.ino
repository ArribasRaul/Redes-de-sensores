#include <SPI.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "WLAN_XD";
const char* password = "975230712";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

IPAddress server(192,168,1,134);  // Google
int puerto = 8080;

// Initialize the client library
WiFiClient client;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  client.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");  
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("\nStarting connection...");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  // if you get a connection, report back via serial:
  if (client.connect(server, puerto)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  printLocalTime();
  delay(1000);
}