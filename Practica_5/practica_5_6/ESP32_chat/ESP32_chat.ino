#include <SPI.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "WLAN_XD";
const char* password = "975230712";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

IPAddress server(192,168,1,133);  // mi pc
int puerto = 8888;

// Initialize the client library
WiFiClient client;

int empezar = 0;
char inChar;
String data;
int len = 0;
byte buffer[512];

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
  if (client.available()){
    len = client.available(); 
    client.read(buffer,len);
    data = "";
    for (int i = 0; i < len; i++) {
      data += (char)buffer[i];
    }
  }

  if (data == "Start"){
    empezar = 1;
  }

  else if (data == "Stop"){
    empezar = 0;
  }

  if (empezar == 1){
    printLocalTime();
    delay(500);
  }
}