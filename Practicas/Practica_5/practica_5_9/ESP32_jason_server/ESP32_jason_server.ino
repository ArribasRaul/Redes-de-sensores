#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
#include "octocat.h"
#include <ArduinoJson.h>

#define WIFI_SSID "Phone_1_3262"
#define WIFI_PASS "raulhermozo"

char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";

int interval = 0; // primera medida
int previousMillis = 0;
int temp = 0;
int temp_sig = 0;

int contador = 0;

// you can pass a FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

void setup()
{
  Serial.begin( 115200 );

  randomSeed(analogRead(0));
  temp = random(0,35);

  WiFi.begin( WIFI_SSID, WIFI_PASS );
  
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("\nStarting connection...");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ftp.OpenConnection();

  // Create the file new and write a string into it
  ftp.ChangeWorkDir("/rsense/785300");
  ftp.InitFile("Type A");
  ftp.NewFile("grupo01_11032024.json");
  //ftp.NewFile("prueba.txt");

}

void loop(){
  int currentMillis = millis();
  if (contador <= 6){
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
      contador = contador + 1;
      if (contador > 6){
        ftp.CloseFile();
        ftp.CloseConnection();
      }
    }
    interval = 10000; // 10 segundos
  }
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
  ftp.Write(output.c_str());
  ftp.Write("\n");
}


