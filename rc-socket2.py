import socket
import sys
import random
from thread import *

HOST = ''
PORT = random.randint(10000, 65535)
print "PORT: {}".format(PORT)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'socket created'

try:
    s.bind((HOST, PORT))
except socket.error, msg:
    print 'Bind failed.  Error code: {} {}'.format(*msg)
    sys.exit()

print 'socket bind complete'

s.listen(10)  # 10 connections
print 'socket now listening'

def clientthread(conn):
    conn.send('Welcome to JJ\'s socket server!')

    while True:
        data = conn.recv(1024)
        if not data:
            break
        reply = random.choice(("I'm a socket!", "Hey!", "Hook me up to the car already!"))
        conn.sendall(reply)

    conn.close()

while 1:
    conn, addr = s.accept()
    print "Connected with {} at {}".format(*addr)

    # from threading
    start_new_thread(clientthread, (conn,))

s.close()
