from socket import * 
import select 
import sys 
  
clientSocket = socket(AF_INET, SOCK_STREAM) 

if len(sys.argv) != 3: 
    print ("Please enter IP address and port number")
    exit() 
serverName = str(sys.argv[1]) 
serverPort = int(sys.argv[2]) 

clientSocket.connect((serverName, serverPort)) 
  
while True: 
  
    sockets_list = [sys.stdin, clientSocket] 
  
    readSockets, writeSocket, errorSocket = select.select(sockets_list,[],[]) 
  
    for socks in readSockets: 
        if socks == clientSocket: 
            message = socks.recv(2048) 
            print(message.decode("utf-8")) 
        else: 
            message = sys.stdin.readline() 
            clientSocket.send(message.encode())  
            sys.stdout.flush() 
clientSocket.close()