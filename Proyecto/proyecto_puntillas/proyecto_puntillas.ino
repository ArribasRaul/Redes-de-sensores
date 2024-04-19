//#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

#define T_SAMPLE_MS 50
#define D_ACEL_THRESHOLD 0.5  // Umbral derivada aceleración
#define D_ACEL_THRESHOLD_PARADA 0.4

// Estados
const int ABAJO = 1;
const int SUBIDA = 2;
const int ARRIBA = 3;
const int BAJADA = 4;
int estado = ABAJO;

// Tiempos
int inicio_subida = 0;
int parada_subida = 0;
int tiempo_subida = 0;
int inicio_puntillas = 0;
int parada_puntillas = 0;
int tiempo_puntillas = 0;

// Medidas angulos
float angulo = 0;
float angulo_maximo = 0;
float angulo_minimo = 999;
float angulo_promedio = 0;
float suma_angulo = 0;
float cuenta_angulo = 0;

// BLEService acelService("6282a2a1-5f99-430e-abda-ba09b255bdd4");
// BLEByteCharacteristic acelChar("68f6b451-210d-4dda-9dea-bd8ead4bc544", BLERead | BLENotify);
// BLEDescriptor acelDes("0x2901", "Aceleracion");

void setup() {
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // if (!BLE.begin()) {
  //   Serial.println("starting Bluetooth® Low Energy failed!");
  //   return;
  // }
  // BLE.setLocalName("CuentaPasos");

  // BLE.setAdvertisedService(acelService);
  // acelChar.addDescriptor(acelDes);
  // acelService.addCharacteristic(acelChar);
  // BLE.addService(acelService);

  // BLE.advertise();
}

void loop() {
  static float acelX = 0.0, acelY = 0.0, acelZ = 0.0;
  static float acelXAnt = 0.0, acelYAnt = 0.0, acelZAnt = 0.0;
  static float dAcelX = 0.0, dAcelY = 0.0, dAcelZ = 0.0;
  static float giroX = 0.0, giroY = 0.0, giroZ = 0.0;

  if (IMU.accelerationAvailable()) {
    float x, y, z;
    IMU.readAcceleration(x, y, z);
    acelX = -x;
    acelY = -z;
    acelZ = y;
  }

  if (IMU.gyroscopeAvailable()){
    float x, y, z;
    IMU.readGyroscope(x, y, z);
    giroX = x;
    giroY = z;
    giroZ = -1*y;
  }
  
  //Hacer derivada acel
  dAcelX = (acelX - acelXAnt) / (T_SAMPLE_MS/1000.0);
  dAcelY = (acelY - acelYAnt) / (T_SAMPLE_MS/1000.0);
  dAcelZ = (acelZ - acelZAnt) / (T_SAMPLE_MS/1000.0);

  acelXAnt = acelX;
  acelYAnt = acelY;
  acelZAnt = acelZ;

  angulo = angulo - giroY; // Eje y giro positivo hacia abajo 

  if(estado == ABAJO){
    angulo = 0;
    angulo_maximo = 0;
    angulo_minimo = 0;
    angulo_promedio = 0;

    if(dAcelX > D_ACEL_THRESHOLD){
      Serial.println("Cambio de estado a subiendo");
      inicio_subida = millis();
      estado = SUBIDA;
    }
  }
  
  else if(estado == SUBIDA){
    if(dAcelX < D_ACEL_THRESHOLD_PARADA && dAcelX > -1*D_ACEL_THRESHOLD_PARADA){
      Serial.println("Cambio de estado a arriba");
      parada_subida = millis();
      tiempo_subida = parada_subida - inicio_subida;
      Serial.println(tiempo_subida);
      estado = ARRIBA;
    }
  }

  else if(estado == ARRIBA){
    inicio_puntillas = parada_subida;
    
    if(angulo_maximo < angulo){
      angulo_maximo = angulo;
    }

    if(angulo_minimo > angulo){
      angulo_minimo = angulo;
    }

    if(dAcelX < -1*D_ACEL_THRESHOLD){
      Serial.println("Cambio de estado a bajando");
      parada_puntillas = millis();
      tiempo_puntillas = parada_puntillas - inicio_puntillas;
      Serial.println(tiempo_puntillas);
      estado = BAJADA;
    }
  }

  else if(estado == BAJADA){
    if(dAcelX < D_ACEL_THRESHOLD_PARADA && dAcelX > -1*D_ACEL_THRESHOLD_PARADA){
      Serial.println("Cambio de estado a abajo");
      estado = ABAJO;
    }
  }

  Serial.println(angulo);

  delay(T_SAMPLE_MS);
}
