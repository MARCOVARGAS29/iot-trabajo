
import socket
import time
from _thread import start_new_thread

ServerSocket = socket.socket()
host = '0.0.0.0'  # Escucha en todas las interfaces de red
port = 65432      # Cambiado a un puerto diferente
distance = -1

try:
    ServerSocket.bind((host, port))
except socket.error as e:
    print(str(e))

ServerSocket.listen(5)
ServerSocket.settimeout(10)  # Establece un tiempo de espera de 10 segundos

def threaded_client(connection):

    global distance
    
    while True:
        try:
            data = connection.recv(2048).decode('utf-8')
            if not data:
                
                break
            if data.startswith('distance='):
                try:
                    distance = int(data[9:].strip())  # Usa strip() para eliminar espacios en blanco y '\n'
                    print('distance=' + str(distance))
                    reply = "ACK\n"
                except ValueError:
                    print("Error al convertir la distancia a entero")
                    continue  # Salta al siguiente ciclo del bucle
            elif data.startswith('GET'):
                if distance < 10:
                    reply = "0\n"
                    print("Encendiendo LED Azul")
                elif distance < 20:
                    reply = "1\n"
                    print("Encendiendo LED Rojo")
                elif distance < 30:
                    reply = "2\n"
                    print("Encendiendo LED Blanco")
                else:
                    reply = "3\n"
                    print("Encendiendo LED Azul 2")
                connection.sendall(reply.encode('utf-8'))
        except ConnectionResetError:
            print("Conexión restablecida por el host remoto")
            break  # Salir del bucle while para cerrar la conexión
    connection.close()

start_time = time.time()
while True:
    try:
        Client, address = ServerSocket.accept()
        print('Conexión desde: ' + address[0] + ':' + str(address[1]))
        start_new_thread(threaded_client, (Client,))
    except socket.timeout:
        if time.time() - start_time > 10:
            print("Cerrando el servidor debido a un tiempo de espera.")
            break

ServerSocket.close()
