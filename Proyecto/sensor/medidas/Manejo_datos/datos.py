entrada = open("medida9_muy_corto.txt", "r")
salidaAcel = open("medida9_ace.csv", "a")
salidaGyro = open("medida9_gyro.csv", "a")

salidaAcel.write("x, y, z\n")
salidaGyro.write("x, y, z\n")

indice = 1
for linea in entrada:
    if indice == 1:
        salidaAcel.write(linea)
        indice = 2
    elif indice == 2:
        salidaGyro.write(linea)
        indice = 1

entrada.close()
salidaAcel.close()
salidaGyro.close()
