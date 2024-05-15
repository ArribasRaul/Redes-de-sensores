# Práctica 5 - Comunicaciones WIFI y stack IP con ESP32 

## Objetivos de la Práctica

- Poner en práctica los conocimientos de redes inalámbricas WiFi.
- Manejar comunicaciones IP a bajo nivel mediante sockets y entre diversas plataformas: PC, Móvil, CLOUD, Sensor.
- Manejar protocolos de alto nivel como HTTP, FTP, NTP, MQTT y estándares de interoperabilidad como SENML.

## Contenido de la Carpeta

1. **Practica_5_1**: Conexión a la red WiFi y comprobación de la conectividad con Google mediante un ping.
2. **Practica_5_2**: Sincronización de hora utilizando un servidor NTP (Network Time Protocol).
3. **Practica_5_4**: Chat entre dos sockets con una aplicación PC (SocketTest) o una aplicación móvil (Simple TCP Socket Tester).
4. **Practica_5_5**: Sustitución de uno de los extremos por un módulo hardware siendo cliente y envío de la hora local cada segundo, con control de inicio y parada.
5. **Practica_5_6**: Adaptación del código de la Práctica 3 al ESP32 para conectarse a la WiFi, a un servidor TCP y enviar datos del acelerómetro.
6. **Practica_5_7**: Montaje de un servidor WEB que muestre la hora y tenga un botón para resetear la hora a las 0:00.
7. **Practica_5_8**: Generación de un archivo JSON basado en el estándar SenML que se genere cada 10 segundos, conteniendo datos de temperatura inventados con sus unidades y marca temporal.
8. **Practica_5_9**: Subida de los archivos generados a un servidor FTP con un nombre que sigue el formato grupoXX_ddmmss.json.
9. **Practica_5_10**: Subida de datos a un servicio gratuito proporcionado por Adafruit, utilizando MQTT para comunicarse con el servidor.

