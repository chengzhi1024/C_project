# coding: utf-8
import threading
import socket
from utils.SingletonQueue import SingletonQueue
from utils.SingletonTcpSocket import SingletonTcpSocket

class SocketClientThread(threading.Thread):
    def __init__(self, ip, port):
        threading.Thread.__init__(self)
        self.thread_stop = False
        self.ip = ip
        self.port = port

    def run(self):
        # SingletonTcpSocket().connect(self.ip, self.port)
        # s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        SingletonTcpSocket().connect(self.ip, self.port)
        while not self.thread_stop:
            task = SingletonQueue().get()
            print("task recv:%s ,task No:%d" % (task[0], task[1]))
            SingletonTcpSocket().sock.send(bytes(task[0], encoding = "utf8"))
            str = SingletonTcpSocket().sock.recv(1024)
            print('recv from server:%s' % str)

    def stop(self):
        self.thread_stop = True
