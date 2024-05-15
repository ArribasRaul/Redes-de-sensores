# Repositorio Redes de Sensores Electónicos

Este repositorio contiene una serie de proyectos desarrollados como parte del curso de redes de sensores electónicos. Los proyectos abarcan una variedad de temas, desde la programación de microcontroladores hasta la implementación de sistemas de comunicaciones inalámbricas y el desarrollo de firmware utilizando diferentes tecnologías y plataformas.

## Objetivos Generales

- Explorar y comprender diferentes tecnologías y plataformas utilizadas en redes de sensores electónicos.
- Desarrollar habilidades en la programación de microcontroladores y sistemas embebidos.
- Practicar el diseño y la implementación de proyectos electrónicos desde cero.

## Contenido del Repositorio

### Práctica 1 - Arquitectura Arduino Nano 33 BLE
- **Practica_1**: Proyecto que aborda el uso de un Arduino Nano 33 BLE para diversas tareas, como la lectura de ADC, generación de PWM, comunicación UART, entre otros.

    1. Lectura del ADC y generación de PWM.
    2. Comunicación UART para gestionar periféricos mediante un protocolo específico.
    3. Configuración de interrupciones mediante timers hardware.
    4. Comunicación I2C entre dos placas para el intercambio de datos de sensores.
    5. Utilización de sensores internos como acelerómetro, giróscopo y magnetómetro.

### Práctica 2 - Desarrollo de ML embebido con Edge Impulse
- **Practica_2**: Implementación de proyectos utilizando Edge Impulse para el análisis de señales y reconocimiento de patrones, integrado con Arduino Nano 33 BLE.

    1. Desarrollo de modelos de machine learning para reconocer palabras y gestos utilizando Edge Impulse.
    2. Implementación de los modelos en el Arduino Nano 33 BLE para tareas específicas.

### Práctica 3 - Comunicación, gestión y representación de datos de sensores con Python
- **Practica_3**: Desarrollo de programas en Python para la gestión de datos serie procedentes de un Arduino Nano 33 BLE, incluyendo almacenamiento, procesamiento y representación gráfica.

    1. Desarrollo de programas en Python para la gestión de datos serie, incluyendo almacenamiento, procesamiento y representación gráfica.
    2. Conexión a través del puerto serie y comunicación bidireccional entre el Arduino y el PC.

### Práctica 4 - Comunicaciones BLE y Bluetooth
- **Practica_4**: Proyectos relacionados con las comunicaciones Bluetooth, incluyendo la configuración de dispositivos periféricos, chat mediante Bluetooth Classic y más.

    1. Configuración de dispositivos periféricos y establecimiento de comunicación BLE.
    2. Implementación de un chat mediante Bluetooth Classic con el ESP32.
    3. Integración con servicios de terceros como Adafruit IO para la subida de datos a la nube.

### Práctica 5 - Comunicaciones WIFI y stack IP con ESP32
- **Practica_5**: Desarrollo de proyectos relacionados con redes inalámbricas WiFi, comunicación TCP/IP, HTTP, FTP, MQTT, entre otros.

    1. Conexión a redes WiFi y sincronización de hora utilizando NTP.
    2. Implementación de chat mediante sockets TCP/IP.
    3. Generación de archivos JSON y subida a servidores FTP y servicios en la nube como Adafruit IO.

### Práctica 6 - Diseño de firmware basado en sistema operativo de tiempo real (RTOS)
- **Practica_6**: Implementación de programas utilizando FreeRTOS para la programación de sistemas embebidos en tiempo real.
    
    1. Creación de tareas y gestión de concurrencia.
    2. Uso de semáforos y colas para la sincronización entre tareas.
    3. Implementación de un firmware basado en FreeRTOS para el control de dispositivos.
