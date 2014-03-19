import socket


s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name

myport = 17845                # Reserve a port for your service.

s.bind((host, myport)) 
s.listen(5)                 # Now wait for client connection.

def main():
  loop = True
  while loop:  
    c, addr = s.accept()     # Establish connection with client.
    print 'Got connection from', addr
    data = c.recv(1024)

    if data:
      print data

    if data == "x":
      loop = False
      c.send('Thank you for connecting')
    c.close()
  pass

if __name__ == "__main__":
  main()
