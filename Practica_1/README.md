# Práctica 1 - Arquitectura Arduino Nano 33 BLE 

## Objetivos de la Práctica

- Familiarización con el sistema de desarrollo Arduino Nano 33 BLE.
- Manejo de los periféricos típicos de cualquier microcontrolador: ADC, PWM, timers, GPIOs.

## Contenido de la Carpeta

1. **blink**: Ejemplo básico para comprobar el funcionamiento del Arduino mediante el parpadeo de un LED.
2. **Practica_1_1**: Lectura del valor del ADC y visualización por consola con temporización por software.
3. **Practica_1_2**: Utilización de timers hardware para generar interrupciones y lecturas periódicas del ADC.
4. **Practica_1_3**: Generación de salida PWM proporcional a la lectura del ADC.
5. **Practica_1_4**: Gestión de periféricos mediante UART y protocolo de comunicación definido.
6. **Practica_1_5**: Comunicación entre dos placas Arduino mediante el protocolo I2C para control de un LED externo.
7. **Practica_1_6**: Muestreo de datos de sensores integrados y envío por UART.
8. **Practica_1_7**: Integración de las prácticas 1_6 y 1_5 para comunicación entre placas Arduino y envío de datos de sensores.

Cada práctica presenta un desafío específico relacionado con el desarrollo de sistemas electrónicos utilizando microcontroladores Arduino. Se recomienda abordarlas en orden numérico para una progresión adecuada en la complejidad de los ejercicios.

## Información Adicional sobre el Arduino Nano 33 BLE

La placa de desarrollo Arduino Nano 33 BLE utiliza el potente procesador nRF52840 de Nordic Semiconductors, que cuenta con las siguientes características:

- ARM Cortex-M4F 64MHz
- 32 bits
- Unidad de coma flotante (FPU)
- 1 MB Flash
- 256 KB RAM

Además, incluye el módulo NINA-B306, que proporciona comunicaciones Bluetooth 5. Para la captura de datos, la placa cuenta con varios sensores integrados, incluyendo:

- IMU de 9 grados de libertad (LSM9DS1) con resolución de 16 bits.
- Micrófono digital omnidireccional (MP34DT05).
- Sensor de luz ambiente y color (APDS9960) que permite la detección de gestos, proximidad, etc.
- Sensor de presión (LPS22HB).
- Sensor capacitivo de temperatura y humedad (HTS221).
