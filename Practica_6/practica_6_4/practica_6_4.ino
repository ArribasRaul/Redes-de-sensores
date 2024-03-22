#include <ArduinoBLE.h>

BLEService ledService("180A"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLENotify);
BLEByteCharacteristic switchCharacteristic2("2A58", BLEWrite);

int activo = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("ESP_Raul");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  ledService.addCharacteristic(switchCharacteristic2);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characteristic:
  switchCharacteristic.writeValue(0);
  switchCharacteristic2.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      if(activo == 1){
        switchCharacteristic.writeValue(random(0,35));
        delay(1000);
      }

      if (switchCharacteristic2.written()) {
        switch (switchCharacteristic2.value()) {   // any value other than 0
          case 01:
            Serial.println(F("Acelerometro on"));
            activo = 1;
            break;
          default:
            Serial.println(F("Acelerometro off"));
            activo = 0;
            break;
        }
      }
    }
    

  }
}