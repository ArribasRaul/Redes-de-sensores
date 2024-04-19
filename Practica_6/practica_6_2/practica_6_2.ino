int Led = 5;
int inercia = 0;

void setup() {
  Serial.begin(112500);
  delay(1000);

  xTaskCreate(Tarea1,"Tarea1",10000,NULL,1,NULL);
  xTaskCreate(Tarea2,"Tarea2",10000,NULL,1,NULL);

  pinMode(Led, OUTPUT);
}

void loop() {
  delay(1000);
}

void Tarea1( void * parameter ){
  while(true){
    inercia = random(0, 100);
    vTaskDelay(100); // Ejecuta esta tarea cada 100 milisegundos
  }
  vTaskDelete(NULL);  // Sin esto se reinicia el micro
}

void Tarea2( void * parameter){
  while(true){
    Serial.println(inercia);
    digitalWrite(Led, HIGH);
    Serial.println("LED on");
    delay(200);
    digitalWrite(Led, LOW);
    Serial.println("LED off");
    vTaskDelay(800); // Ejecuta esta tarea cada 800 milisegundos
  }
  vTaskDelete(NULL);  // Sin esto se reinicia el micro
}