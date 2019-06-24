# coding utf-8

from utils.Singleton import singleton
from utils.SingletonQueue import SingletonQueue
from PyQt5.QtNetwork import QTcpSocket
from PyQt5.QtNetwork import QNetworkProxy
import socket

@singleton
class SingletonTcpSocket():
    def __init__(self):
        self.sock = None
        self.isConnectedToServer = False

    def connect(self, ip, port):
        if self.isConnectedToServer:
            return True
        # self.sock = QTcpSocket()
        # reslove error of proxy type is invalid for this operation
        # proxy = QNetworkProxy(QNetworkProxy.NoProxy)
        # self.sock.setProxy(proxy)

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # self.sock.connected.connect(self.on_socket_connected)
        # self.sock.disconnected.connect(self.on_socket_disconnected)
        # self.sock.readyRead.connect(self.on_socket_receive)
        # self.sock.bytesWritten.connect(self.on_socket_transmit)
        # self.sock.error.connect(self.on_socket_error)

        # self.sock.connectToHost(ip, port)
        # if not self.sock.waitForConnected(25000):
        #     print('Error connect:%s' % self.sock.errorString())
        #     return False

        self.sock.connect((ip, port))

        print("connect successful")
        self.isConnectedToServer = True
        return True

    def on_socket_connected(self):
        print("on_socket_connected")

    def on_socket_disconnected(self):
        print("on_socket_disconnected")

    def on_socket_receive(self):
        rxData = self.sock.readAll()
        print(rxData)

    def on_socket_transmit(self, numBytes):
        print(" (" + str(numBytes) + " bytes)\n")

    def on_socket_error(self, error):
        print('on_socket_error %s' % self.sock.errorString())
        self.sock.close()

    # noinspection PyBroadException
    def disconnect(self):
        if not self.isConnectedToServer:
            return
        try:
            self.sock.connected.disconncet()
        except:
            pass
        try:
            self.sock.disconnected.disconncet()
        except:
            pass
        try:
            self.sock.readyRead.disconncet()
        except:
            pass
        try:
            self.sock.bytesWritten.disconncet()
        except:
            pass
        self.sock.close()

if __name__ == "__main__":
    SingletonTcpSocket().connect('192.168.126.130', 9999)
    SingletonQueue().get()