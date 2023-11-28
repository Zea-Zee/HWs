import numpy as np
import torch
import torch.utils.data as data
import time


BLUE = '\033[94m'
CYAN = '\033[96m'
GREEN = '\033[92m'
RED = '\033[91m'
RESET = '\033[0m'


class GeneratorDataset(data.Dataset):
    def __init__(self, in_size, out_size, num_samples, func='sin'):
        super().__init__()
        self.num_samples = num_samples
        self.in_size = in_size
        self.out_size = out_size
        self.func = func

    def __getitem__(self, index):
        x = torch.rand(self.in_size)
        if self.func == 'sin':
            x = torch.sin(x)
        elif self.func == 'cos':
            x = torch.cos(x)
        y = x[:self.out_size].clone()
        return x, y

    def __len__(self):
        return self.num_samples


class Model:
    def __init__(self, input_size=256, hidden1_size=64, hidden2_size=16, output_size=4, learning_rate=0.01, name="Barebone"):
        self.name = name
        self.weight_1 = torch.zeros(input_size, hidden1_size, requires_grad=True)
        self.bias_1 = torch.zeros(hidden1_size, requires_grad=True)
        self.weight_2 = torch.zeros(hidden1_size, hidden2_size, requires_grad=True)
        self.bias_2 = torch.zeros(hidden2_size, requires_grad=True)
        self.weight_3 = torch.zeros(hidden2_size, output_size, requires_grad=True)
        self.bias_3 = torch.zeros(output_size, requires_grad=True)
        self.lr = learning_rate

    def forward(self, x):
        x = x @ self.weight_1
        x += self.bias_1
        x = torch.relu(x)

        x = x @ self.weight_2
        x += self.bias_2
        x = torch.tanh(x)

        x = x @ self.weight_3
        x += self.bias_3
        x = torch.softmax(x, dim=1)
        return x


    def backward(self, x, y):
        y_pred = self.forward(x)
        # Вычисляем функцию ошибок
        loss = torch.mean((y_pred - y) ** 2)
        # Вычисляем градиенты
        loss.backward()
        # Делаем шаг градиентного спуска по матрице весов
        self.weight_1.data -= self.lr * self.weight_1.grad.data
        self.weight_2.data -= self.lr * self.weight_2.grad.data
        self.weight_3.data -= self.lr * self.weight_3.grad.data

        self.bias_1.data -= self.lr * self.bias_1.grad.data
        self.bias_2.data -= self.lr * self.bias_2.grad.data
        self.bias_3.data -= self.lr * self.bias_3.grad.data
        # b.data -= lr * b.grad.data
        # b.grad.data.zero_()
        # обнуляем градиенты
        self.weight_1.grad.data.zero_()
        self.weight_2.grad.data.zero_()
        self.weight_3.grad.data.zero_()
        return loss.data


def learn_model(model, dataset, stop_loss):
    start = time.time()
    # dataloader = data.DataLoader(dataset, batch_size=32)
    dataloader = data.DataLoader(dataset, batch_size=32)
    for x, y in dataloader:
        break
    isFirst = True
    counter = 0
    for x, y in dataloader:
        mse = model.backward(x, y)
        if isFirst:
            isFirst = False
            print(f"first loss is {GREEN}{mse}{RESET}")
        if mse < stop_loss and counter > 1000:
            print(f"Model {BLUE}{model.name}{RESET} ended learning with loss {GREEN}{mse}{RESET} on {counter}'th step\n"
            f"For one iteration it took {CYAN}{(time.time() - start) * 1000 / counter}ms{RESET}")
            return
        if counter % 100 == 0:
            print(f"Loss on {counter}'th step is {mse}")
        if counter >= 9999:
            break
        counter += 1
    print(f"Model {BLUE}{model.name}{RESET} have ended learning {RED}due to end of data{RESET} with loss {GREEN}{mse}{RESET} on {counter}'th step\n"
          f"For one iteration it took {CYAN}{(time.time() - start) * 1000 / counter}ms{RESET}")

model = Model()
learn_model(model, GeneratorDataset(256, 4, 320000), 0.0001)