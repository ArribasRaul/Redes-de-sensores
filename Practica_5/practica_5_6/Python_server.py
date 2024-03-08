import socket
import time 

# Set up a TCP/IP server
tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
 
# Bind the socket to server address and port 81
server_address = ('192.168.1.133', 8888)
tcp_socket.bind(server_address)
 
# Listen on port 81
tcp_socket.listen(1)
 
contador = 0

while True:

    print("Waiting for connection")
    connection, client = tcp_socket.accept()
 
    try:
        print("Connected to client IP: {}".format(client))
         
        # Receive and print data 32 bytes at a time, as long as the client is sending something
        while True:
            connection.sendall("Start".encode("ascii"))
            data = connection.recv(40)
            dataASCII = data.decode("ascii")
            dataASCIIsin = dataASCII.strip()
            #print("Received data: {}".format(dataASCIIsin))
            print("Received data: {}".format(dataASCII))
            contador = contador + 1
            
            if not data:
                break
            
            if contador >= 15:
                connection.sendall("Stop".encode("ascii"))
                break
 
    finally:
        connection.close()

