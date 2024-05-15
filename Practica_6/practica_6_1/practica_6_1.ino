int Led = 5;

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
    Serial.println("Hola mundo");
    vTaskDelay (1000); //Ejecuta esta tarea cada 1000 milisegundos
  }
  vTaskDelete(NULL);  // sin esto se reinicia el micro
}

void Tarea2( void * parameter){
  while(true){
    digitalWrite(Led, HIGH);
    Serial.println("LED on");
    delay(200);
    digitalWrite(Led, LOW);
    Serial.println("LED off");
    delay(200);
  }
  vTaskDelete(NULL);
}