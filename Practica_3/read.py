import matplotlib.pyplot as plt
import serial, time

archivo=open("datos2.txt",'w')
arduino = serial.Serial('COM4', 9600)

fig, ax = plt.subplots()
ejex = []
ejey = []
X = []
Y = []
Z = []


contador = 1
    
while contador < 101:
    ejex.append(contador)
    
    data = arduino.readline()
    print(data)
    dataASCII = data.decode("ascii")
    archivo.write(dataASCII)
    
    datos = str(dataASCII)
    print(datos)
    
    datos_separados = datos.split(";")
    X.append(datos_separados[0])
    Y.append(datos_separados[1])
    Z.append(datos_separados[2])
    
    lectura = {'X':X, 'Y':Y, 'Z':Z}
    ax.plot(ejex, lectura['X'], color = 'tab:red')
    ax.plot(ejex, lectura['Y'], color = 'tab:green')
    ax.plot(ejex, lectura['Z'], color = 'tab:blue')
    plt.draw()
    plt.pause(0.01)
    
    contador += 1

arduino.close()
archivo.close()










