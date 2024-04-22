#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

#define T_SAMPLE_MS 50

#define D_ACEL_THRESHOLD 0.3  // Umbral derivada aceleración
#define D_ACEL_THRESHOLD_PARADA 0.15
#define ACEL_THRESHOLD_ABAJO 0.05

// Estados
const int ST_ABAJO = 1;
const int ST_SUBIDA = 2;
const int ST_ARRIBA = 3;
const int ST_BAJADA = 4;
int estado = ST_ABAJO;

// Tiempos
int inicio_subida = 0;
int parada_subida = 0;
int tiempo_subida = 0;
int inicio_puntillas = 0;
int parada_puntillas = 0;
int tiempo_puntillas = 0;

// Medidas angulos
#define ANGULO_OFFSET 20
float angulo = ANGULO_OFFSET;
float angulo_maximo = 0;
float angulo_minimo = 999;
float angulo_promedio = 0;
float suma_angulo = 0;
float cuenta_angulo = 0;

// Buffer medidas. Tamaño de ventana de filtro BUFFER_SIZE
#define BUFFER_SIZE 5
float bufferAcelX[BUFFER_SIZE];

BLEService movService("6282a2a1-5f99-430e-abda-ba09b255bdd4");

BLECharCharacteristic procesoChar("99ee8226-8c9e-4972-b4f5-ea7af1caa0b5", BLEWrite);
BLEDescriptor procesoDes("0x2901", "Empezar proceso");
BLECharCharacteristic resetChar("7b14e65a-8610-4cd5-8653-5a3e0020204e", BLEWrite);
BLEDescriptor resetDes("0x2901", "Reset");

BLEIntCharacteristic maxChar("17cc3d4f-69c7-4e00-a954-0346ecbb22c7", BLERead | BLENotify);
BLEDescriptor maxDes("0x2901", "Angulo maximo");
BLEIntCharacteristic minChar("6a8adb71-4b3b-421f-b5e1-e74bad969122", BLERead | BLENotify);
BLEDescriptor minDes("0x2901", "Angulo minimo");
BLEIntCharacteristic medioChar("fdee3592-43f5-40ac-b408-2a15a0b4413c", BLERead | BLENotify);
BLEDescriptor medioDes("0x2901", "Angulo medio");
BLECharCharacteristic subidaChar("64247fd3-2d0e-461f-ac13-8651d2ba991e", BLERead | BLENotify);
BLEDescriptor subidaDes("0x2901", "Tiempo de subida");
BLECharCharacteristic altoChar("ddc902e8-9d9f-4db6-ba57-76609f6d2adb", BLERead | BLENotify);
BLEDescriptor altoDes("0x2901", "Tiempo en alto");

BLEIntCharacteristic anguloChar("38b48b19-373f-4425-a790-42df14c08300", BLERead | BLENotify);
BLEIntCharacteristic estadoChar("c4346fa0-d0ee-4c24-817f-0fab75a7fd23", BLERead | BLENotify);

void setup() {
  Serial.begin(115200);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  // Iniciar buffers a 0
  bufferInit(bufferAcelX, BUFFER_SIZE);

  // BLE
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
    while (1)
      ;
  }
  BLE.setLocalName("DispositivoDoCaminaçao");

  BLE.setAdvertisedService(movService);

  subidaChar.addDescriptor(subidaDes);
  movService.addCharacteristic(subidaChar);

  altoChar.addDescriptor(altoDes);
  movService.addCharacteristic(altoChar);

  medioChar.addDescriptor(medioDes);
  movService.addCharacteristic(medioChar);

  minChar.addDescriptor(minDes);
  movService.addCharacteristic(minChar);

  maxChar.addDescriptor(maxDes);
  movService.addCharacteristic(maxChar);

  resetChar.addDescriptor(resetDes);
  movService.addCharacteristic(resetChar);

  procesoChar.addDescriptor(procesoDes);
  movService.addCharacteristic(procesoChar);

  movService.addCharacteristic(anguloChar);
  movService.addCharacteristic(estadoChar);

  BLE.addService(movService);

  BLE.advertise();
}

void loop() {
  static uint8_t procesando = 0;
  static int deltaTimeMs = 0;
  static BLEDevice central = BLE.central();
  int resetear = 0;
  int startTime = millis();

  static int debug_conectado = 0;

  if (central) {
    if (debug_conectado == 0) {
      debug_conectado = 1;
      Serial.println("Conexion entrante");
    }
    if (central.connected()) {
      if (procesoChar.written()) {
        procesando = procesoChar.value();
        Serial.print("Proceso recibido ");
        Serial.println(procesando);
      }
      if (resetChar.written()) {
        resetear = resetChar.value();
        resetChar.writeValue(0);
        Serial.println("Reset recibido");
      }
    }
  } else {
    if (debug_conectado == 1) {
      debug_conectado = 0;
      Serial.println("Conexion cerrada");
    }
    central = BLE.central();
  }


  if (resetear == 1) {
    Serial.println("Entrado en reset");
    angulo = ANGULO_OFFSET;
    angulo_maximo = 0;
    angulo_minimo = 999;
    angulo_promedio = 0;
    suma_angulo = 0;
    cuenta_angulo = 0;
    estado = ST_ABAJO;
    bufferInit(bufferAcelX, BUFFER_SIZE);
    resetear = 0;
  }

  if (procesando == 1) {
    procesado();
  }

  deltaTimeMs = millis() - startTime;

  if (T_SAMPLE_MS - deltaTimeMs >= 0)
    delay(T_SAMPLE_MS - deltaTimeMs);
}

void procesado() {
  float acelX = 0.0, acelY = 0.0, acelZ = 0.0;
  float giroX = 0.0, giroY = 0.0, giroZ = 0.0;

  float acelMeanX = 0.0;
  static float acelMeanXAnt = 0.0;
  float dAcelX = 0.0;

  if (IMU.accelerationAvailable()) {
    float x, y, z;
    IMU.readAcceleration(x, y, z);
    acelX = -x;
    acelY = -z;
    acelZ = y;
    bufferAdd(bufferAcelX, BUFFER_SIZE, acelX);
    // bufferPrint(bufferAcelX, BUFFER_SIZE);
    // Serial.print("Mean ");
    // Serial.println(bufferMean(bufferAcelX, BUFFER_SIZE));
  }

  if (IMU.gyroscopeAvailable()) {
    float x, y, z;
    IMU.readGyroscope(x, y, z);
    giroX = x;
    giroY = z;
    giroZ = -y;
  }

  // Derivada acel
  acelMeanX = bufferMean(bufferAcelX, BUFFER_SIZE);
  dAcelX = (acelMeanX - acelMeanXAnt) / (T_SAMPLE_MS / 1000.0);

  acelMeanXAnt = acelMeanX;

  angulo = angulo - giroY * (T_SAMPLE_MS / 1000.0);  // Eje y giro positivo hacia abajo
  anguloChar.writeValue((int)angulo);
  
  maxChar.writeValue((int)angulo_maximo);

  if (angulo_minimo > 800) {
    Serial.print("Angulo MIN: ");
    Serial.println(angulo);
    minChar.writeValue((int)angulo);
  } else {
    Serial.print("Angulo MIN: ");
    Serial.println(angulo_minimo);
    minChar.writeValue((int)angulo_minimo);
  }

  // Serial.print("angulo: ");
  // Serial.println(angulo);

  if (estado == ST_ABAJO) {
    angulo = ANGULO_OFFSET;

    if (acelX > ACEL_THRESHOLD_ABAJO) {
      Serial.println("Cambio de estado a subiendo");
      inicio_subida = millis();
      estadoChar.writeValue((int)ST_SUBIDA);
      estado = ST_SUBIDA;
    }
  }

  else if (estado == ST_SUBIDA) {
    angulo_maximo = 0;
    angulo_minimo = 999;
    angulo_promedio = 0;
    suma_angulo = 0;
    cuenta_angulo = 0;

    if (dAcelX < D_ACEL_THRESHOLD_PARADA && dAcelX > -D_ACEL_THRESHOLD_PARADA) {
      Serial.println("Cambio de estado a arriba");
      parada_subida = millis();
      tiempo_subida = parada_subida - inicio_subida;
      Serial.print("Tiempo subida: ");
      Serial.println(tiempo_subida);
      subidaChar.writeValue(tiempo_subida);
      estadoChar.writeValue((int)ST_ARRIBA);
      estado = ST_ARRIBA;
    }
  }

  else if (estado == ST_ARRIBA) {
    inicio_puntillas = parada_subida;

    if (angulo_maximo < angulo) {
      angulo_maximo = angulo;
      Serial.print("Angulo MAX: ");
      Serial.println(angulo_maximo);
      // maxChar.writeValue((int)angulo_maximo);
    }

    if (angulo_minimo > angulo) {
      angulo_minimo = angulo;
      // if (angulo_minimo > 800) {
      //   Serial.print("Angulo MIN: ");
      //   Serial.println(angulo);
      //   minChar.writeValue(angulo);
      // } else {
      //   Serial.print("Angulo MIN: ");
      //   Serial.println(angulo_minimo);
      //   minChar.writeValue((int)angulo_minimo);
      // }
    }

    suma_angulo += angulo;
    cuenta_angulo++;

    if (dAcelX > D_ACEL_THRESHOLD) {
      Serial.println("Cambio de estado a subiendo");
      estadoChar.writeValue((int)ST_SUBIDA);
      estado = ST_SUBIDA;
    }

    if (dAcelX < -D_ACEL_THRESHOLD) {
      Serial.println("Cambio de estado a bajando");
      parada_puntillas = millis();
      tiempo_puntillas = parada_puntillas - inicio_puntillas;
      Serial.print("tiempo en puntillas: ");
      Serial.println(tiempo_puntillas);
      altoChar.writeValue(tiempo_puntillas);

      angulo_promedio = suma_angulo / cuenta_angulo;
      Serial.print("Angulo promedio: ");
      Serial.println(angulo_promedio);
      medioChar.writeValue(angulo_promedio);

      estadoChar.writeValue((int)ST_BAJADA);
      estado = ST_BAJADA;
    }

  }

  else if (estado == ST_BAJADA) {
    // if (dAcelX < D_ACEL_THRESHOLD_PARADA && dAcelX > -D_ACEL_THRESHOLD_PARADA) {
    if (acelX < ACEL_THRESHOLD_ABAJO && acelX > -ACEL_THRESHOLD_ABAJO) {
      Serial.println("Cambio de estado a abajo");
      estadoChar.writeValue((int)ST_ABAJO);
      estado = ST_ABAJO;
    }
  }
}

void bufferAdd(float *buffer, int bufferLength, float entry) {
  for (int i = bufferLength - 1; i >= 1; i--) {
    buffer[i] = buffer[i - 1];
  }
  buffer[0] = entry;
}

void bufferInit(float *buffer, int bufferLength) {
  for (int i = 0; i < bufferLength; i++) {
    buffer[i] = 0.0;
  }
}

float bufferMean(float *buffer, int bufferLength) {
  float mean = 0.0;
  for (int i = 0; i < bufferLength; i++) {
    mean += buffer[i];
  }
  mean = mean / bufferLength;
  return mean;
}

void bufferPrint(float *buffer, int bufferLength) {
  Serial.print("[");
  for (int i = 0; i < bufferLength; i++) {
    Serial.print(buffer[i]);
    Serial.print(" ");
  }
  Serial.println("]");
}
