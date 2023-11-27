import numpy as np
import time
import torch
import torch.nn as nn
import torch.optim as optim
import torch.utils.data as data


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
        if not type(self.in_size) is tuple:
            x = torch.rand(self.in_size)
            # y = torch.sin(x)
            y = x[:self.out_size].clone()
            return x, y
        else:
            x = torch.rand(self.in_size)
            y = self.generate_y(x)
            return x, y

    def generate_y(self, x):
        # Extract a region from x to create y
        y = x[:self.out_size[0], :self.out_size[1], :self.out_size[2]].clone()
        return y

    def __len__(self):
        return self.num_samples



class SimpleModelSeq(nn.Module):
    def __init__(self, in_ch, out_ch):
        super().__init__()
        self.name = "SimpleModelSeq"
        self.model = nn.Sequential(
            nn.Linear(in_ch, 32, bias=False),
            nn.ReLU(),
            nn.Linear(32, out_ch, bias=False)
        )

    def forward(self, x):
        return self.model(x)




class ThreeLayerModel(nn.Module):
    def __init__(self, in_ch, hidden1_ch, hidden2_ch, out_ch):
        """Регистрация блоков"""
        super().__init__()
        self.name = "ThreeLayerModel"
        self.fc1 = nn.Linear(in_ch, hidden1_ch)         # Полносвязный слой 1
        self.relu = nn.ReLU()       # Функция активации
        self.fc2 = nn.Linear(hidden1_ch, hidden2_ch)  # Полносвязный слой 2
        self.Tanh = nn.Tanh()       # Функция активации
        self.fc3 = nn.Linear(hidden2_ch, out_ch, bias=False)  # Полносвязный слой 2
        self.Softmax = nn.Softmax(dim=1) # Функция активации

    def forward(self, x):
        """Прямой проход"""
        h = self.fc1(x)
        h = self.relu(h)
        h = self.fc2(h)
        h = self.Tanh(h)
        h = self.fc3(h)
        h = self.Softmax(h)
        return h


class DoubleLayerConvModel(nn.Module):
    def __init__(self):
        """Регистрация блоков"""
        super().__init__()
        self.name = "DoubleLayerConvModel"
        self.conv1 = nn.Conv2d(in_channels=3, out_channels=8, kernel_size=2, stride=1, padding=0)
        self.relu1 = nn.ReLU()
        self.pool1 = nn.MaxPool2d(kernel_size=2, stride=2)

        self.conv2 = nn.Conv2d(in_channels=8, out_channels=16, kernel_size=2, stride=1, padding=0)
        self.relu2 = nn.ReLU()
        self.pool2 = nn.MaxPool2d(kernel_size=2, stride=2)

    def forward(self, x):
        x = self.pool1(self.relu1(self.conv1(x)))
        x = self.pool2(self.relu2(self.conv2(x)))
        return x


class CombinedNeuralNetwork(nn.Module):
    def __init__(self, CNN, FCNN):
        super().__init__()
        self.name = "CombinedNeuralNetwork"
        self.CNN = CNN
        self.FCNN = FCNN

    def forward(self, x):
        x = self.CNN(x)
        x = x.view(32, -1)
        # print(x.shape)
        x = self.FCNN(x)
        return x


class CustomDataset(data.Dataset):
    def __init__(self, model, size=320000):
        self.size = size
        self.data = torch.randn((size, 3, 19, 19))  # Example random input data
        self.model = model

    def __len__(self):
        return self.size

    def __getitem__(self, idx):
        input_data = self.data[idx]

        # Generate targets with a relationship to input data (add some noise)
        targets = self.generate_targets(input_data)

        return input_data, targets

    def generate_targets(self, input_data):
        # Use the model's convolutional layers and pooling layers
        x = self.model.pool1(self.model.relu1(self.model.conv1(input_data)))
        x = self.model.pool2(self.model.relu2(self.model.conv2(x)))

        # Global Average Pooling to match the output size
        x = nn.functional.adaptive_avg_pool2d(x, (4, 4))
        return x


class DatasetForCombinedNN(data.Dataset):
    def __init__(self, size=320000):
        self.__size = size
        self.data = torch.randn((size, 3, 19, 19))  # Example random input data

    def __len__(self):
        return self.__size

    def __getitem__(self, idx):
        x = self.data[idx]
        sums = np.zeros(3)
        sums[0] = torch.sum(x[:, :, 0])
        sums[1] = torch.sum(x[:, :, 1])
        sums[2] = torch.sum(x[:, :, 2])
        r = np.eye(4)
        if sums[0] > sums[1]:
            if sums[0] > sums[2]:
                return x, r[0]
            else:
                return x, r[1]
        elif sums[0] < sums[1]:
            if sums[0] > sums[2]:
                return x, r[2]
            else:
                return x, r[3]


def learn_model(model, dataset, stop_loss, is_third=False):
    mae = nn.L1Loss()
    ls = 1
    opt = torch.optim.Adam(model.parameters(), lr=0.0001)
    counter = 0
    isFirst = True
    min_loss = float('inf')
    min_loss_step = float('inf')

    start = time.time()
    # dataloader = data.DataLoader(dataset, batch_size=32)
    dataloader = data.DataLoader(dataset, batch_size=32)
    for x, y in dataloader:
        break
    for x, y in dataloader:
        opt.zero_grad()
        y_pred = model(x)
        loss = mae(y, y_pred)
        loss.backward()
        opt.step()
        loss_val = loss.item()
        ls = loss_val
        if loss_val < min_loss:
            min_loss = loss_val
            min_loss_step = counter
        if isFirst:
            isFirst = False
            print(f"first loss is {GREEN}{loss_val}{RESET}")
        if loss_val < stop_loss and counter > 1000:
            print(f"Model {BLUE}{model.name}{RESET} ended learning with loss {GREEN}{loss_val}{RESET} on {counter}'th step\n"
            f"For one iteration it took {CYAN}{(time.time() - start) * 1000 / counter}ms{RESET}\n"
            f"Min loss during entire learning was {GREEN}{min_loss}{RESET} on {min_loss_step}'th step")
            return
        if counter % 100 == 0:
            print(f"Loss on {counter}'th step is {loss_val}")
        if counter >= 99999:
            break
        counter += 1
    print(f"Model {BLUE}{model.name}{RESET} have ended learning {RED}due to end of data{RESET} with loss {GREEN}{ls}{RESET} on {counter}'th step\n"
          f"For one iteration it took {CYAN}{(time.time() - start) * 1000 / counter}ms{RESET}\n"
          f"Min loss during entire learning was {GREEN}{min_loss}{RESET} on {RED if min_loss_step < counter else GREEN}{min_loss_step}{RESET}'th step")


ex1_model = SimpleModelSeq(64, 10)
learn_model(ex1_model, GeneratorDataset(64, 10, 320000), 0.003)

ex2_model = ThreeLayerModel(256, 64, 16, 4)
learn_model(ex2_model, GeneratorDataset(256, 4, 320000), 0.0001)

ex3_model = DoubleLayerConvModel()
learn_model(ex3_model, CustomDataset(ex3_model), 0.03)

ex4_model = CombinedNeuralNetwork(DoubleLayerConvModel(), ThreeLayerModel(256, 64, 16, 4))
learn_model(ex4_model, DatasetForCombinedNN(), 0.2)