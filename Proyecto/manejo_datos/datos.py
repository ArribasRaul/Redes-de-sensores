entrada = open("medida1.txt", "r")
salidaAcel = open("acel.csv", "a")
salidaGyro = open("gyro.csv", "a")

salidaAcel.write("x, y, z\n")
salidaGyro.write("x, y, z\n")

indice = 0
for linea in entrada:
    if linea.startswith("x"):
        indice = 0
    if indice == 1:
        salidaAcel.write(linea)
    elif indice == 2:
        salidaGyro.write(linea)
    indice += 1

entrada.close()
salidaAcel.close()
salidaGyro.close()
