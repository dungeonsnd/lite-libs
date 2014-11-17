# Echo client program
import socket
import time
import struct
import threading
from multiprocessing import Process

ttl =2

threads_cnt = 1

HOST = '127.0.0.1'
PORT = 18600

def recvall(sock,size):
    torecv =size
    data =''
    while torecv>0:
        d =sock.recv(torecv)
        data =data+d
        torecv = torecv- len(d)
    return data

def thread_proc(nloop):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    n =ttl
    while(n>0):       
        if n==ttl:
            buf ='set a b'
        else:
            buf ='get a'
        
        b =struct.pack('!I',len(buf))
        s.sendall(b+buf)
        print 'sent:',buf
        
        n =n-1
    n =ttl
    while(n>0):
        hdr = recvall(s,4)
        bodylen =struct.unpack('!I',hdr)[0]
        data = recvall(s,bodylen)
        print 'recv:',data
        n =n-1
        
    s.close()

if __name__ == '__main__':
    threads = []
    nloops = range(threads_cnt)
        
    for i in nloops:
        t = Process(target = thread_proc, args = (i,))
        threads.append(t)

    for i in nloops:        # start threads
        threads[i].start()
        
    for i in nloops:        #wait for all
        threads[i].join()   #threads to finish

