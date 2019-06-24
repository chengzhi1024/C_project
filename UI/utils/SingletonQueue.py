# coding utf-8

from utils.Singleton import singleton
import queue


@singleton
class SingletonQueue():
    def __init__(self):
        self.q = queue.Queue(10)

    def put(self, item):
        self.q.put(item)

    def get(self):
        return self.q.get()

if __name__ == "__main__":
    SingletonQueue().put(['{"userId":"009","userPassword":"111sk0D83j&","userName":"我是谁1","userCreateTime":"2019/06/20 10:00:00"}', 1])
    SingletonQueue().put(['{"userId":"009","userPassword":"111sk0D83j&","userName":"我是谁2","userCreateTime":"2019/06/20 10:00:00"}', 1])

    task = SingletonQueue().get()
    print("task recv:%s ,task No:%d" % (task[0], task[1]))
    task = SingletonQueue().get()
    print("task recv:%s ,task No:%d" % (task[0], task[1]))
