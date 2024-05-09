from socket import * 
import select 
from _thread import *
import datetime
  
serverSocket = socket(AF_INET, SOCK_STREAM) 
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1) 
  
serverPort = 12000
serverSocket.bind(('', serverPort)) 

serverSocket.listen(50) 
  
list_of_clients = [] 

def client_thread(connectionSocket, nameofClient): 
  
    connectionSocket.send("You are connected to chatroom.".encode()) 
  
    while True: 
            try: 
                message = connectionSocket.recv(2048) 
                if message: 
                    message_to_send = "<" + nameofClient.decode("utf-8") + "> " + message.decode("utf-8") 
                    broadcast(message_to_send, connectionSocket) 
  
                else: 
                    remove(connectionSocket)                 					 
            except: 
                continue

def broadcast(message, connection): 
    for clients in list_of_clients: 
        if clients!=connection: 
            try: 
                clients.send(message.encode()) 
            except: 
                clients.close() 
                remove(clients) 
    
def remove(connection): 
    if connection in list_of_clients: 
        list_of_clients.remove(connection) 
  
while True: 

    connectionSocket, addr = serverSocket.accept() 
    list_of_clients.append(connectionSocket) 
  
	
    message = ("Please enter your username: ")
    connectionSocket.send(message.encode())
    nameofClient = connectionSocket.recv(2048)
    nameofClient = nameofClient.rstrip()
    if nameofClient:
        print(nameofClient.decode("utf-8") + " connected")
    else:
	    print("Could not receive")
   
    start_new_thread(client_thread,(connectionSocket,nameofClient))     
  
connectionSocket.close() 
serverSocket.close() 