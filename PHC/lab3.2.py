class Queue(list):
    def __init__(self):
        super().__init__()

    def push(self, el):
        self.append(el)

    def pop(self):
        res = self[len(self) - 1]
        del self[len(self) - 1]
        return res

    def back(self):
        return self[len(self) - 1]

    def front(self):
        return self[0]


class Stack(list):
    def __init__(self):
        super().__init__()

    def push(self, el):
        self.append(el)

    def pop(self):
        res = self[len(self) - 1]
        del self[len(self) - 1]
        return res

    def top(self):
        return self[len(self) - 1]


queue = Queue()
queue.push(1)
queue.push(2)
queue.push(3)
queue.push(4)
print(queue)
print(queue.front())
print(queue.back())
print(queue.pop())
print(queue)

print('\n')

stack = Stack()
stack.push(1)
stack.push(2)
stack.push(3)
stack.push(4)
print(stack)
print(stack.top())
print(stack.pop())
print(stack)
