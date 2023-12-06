import cv2
import torch
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import time

BLUE = '\033[94m'
GREEN = '\033[92m'
RED = '\033[91m'
RESET = '\033[0m'

if __name__ == '__main__':
# device = torch.device("cpu")
    device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
    print("computing device is", device)


    # Load CIFAR-10 data and create data loaders with pin_memory
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ])

    batch_size = 160

    trainset = torchvision.datasets.CIFAR10(root='./data', train=True, download=True, transform=transform)
    trainloader = torch.utils.data.DataLoader(trainset, batch_size=batch_size, shuffle=True, num_workers=5, pin_memory=True)
    testset = torchvision.datasets.CIFAR10(root='./data', train=False, download=True, transform=transform)
    testloader = torch.utils.data.DataLoader(testset, batch_size=batch_size, shuffle=False, num_workers=2, pin_memory=True)
    classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

    # get some random training images
    dataiter = iter(trainloader)
    images, labels = next(dataiter)


    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()
            self.name = "Instance"
            self.conv1 = nn.Conv2d(3, 6, 5)
            self.pool = nn.MaxPool2d(2, 2)
            self.conv2 = nn.Conv2d(6, 16, 5)
            self.fc1 = nn.Linear(16 * 5 * 5, 120)
            self.fc2 = nn.Linear(120, 84)
            self.fc3 = nn.Linear(84, 10)

        def forward(self, x):
            x = self.pool(F.relu(self.conv1(x)))
            x = self.pool(F.relu(self.conv2(x)))
            x = torch.flatten(x, 1)  # flatten all dimensions except batch
            x = F.relu(self.fc1(x))
            x = F.relu(self.fc2(x))
            x = self.fc3(x)
            return x


    class ImprovedNet(nn.Module):
        def __init__(self):
            super(ImprovedNet, self).__init__()
            self.name = "Improved"
            self.conv1 = nn.Conv2d(3, 32, 3, padding=1)
            self.conv2 = nn.Conv2d(32, 64, 3, padding=1)
            self.conv3 = nn.Conv2d(64, 128, 3, padding=1)
            self.pool = nn.MaxPool2d(2, 2)
            self.fc1 = nn.Linear(128 * 4 * 4, 512)
            self.fc2 = nn.Linear(512, 256)
            self.fc3 = nn.Linear(256, 10)

        def forward(self, x):
            x = self.pool(F.relu(self.conv1(x)))
            x = self.pool(F.relu(self.conv2(x)))
            x = self.pool(F.relu(self.conv3(x)))
            x = torch.flatten(x, 1)
            x = F.relu(self.fc1(x))
            x = F.relu(self.fc2(x))
            x = self.fc3(x)
            return x


    class AlexNetSmallInput(nn.Module):
        def __init__(self, num_classes=10):
            super(AlexNetSmallInput, self).__init__()
            self.name = "AlexNet"
            self.features = nn.Sequential(
                nn.Conv2d(3, 64, kernel_size=3, stride=2, padding=1),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(kernel_size=2, stride=2),

                nn.Conv2d(64, 192, kernel_size=3, padding=1),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(kernel_size=2, stride=2),

                nn.Conv2d(192, 384, kernel_size=3, padding=1),
                nn.ReLU(inplace=True),

                nn.Conv2d(384, 256, kernel_size=3, padding=1),
                nn.ReLU(inplace=True),

                nn.Conv2d(256, 256, kernel_size=3, padding=1),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(kernel_size=2, stride=2),
            )

            self.avgpool = nn.AdaptiveAvgPool2d((2, 2))  # Уменьшили размерность для поддержки маленьких изображений
            self.classifier = nn.Sequential(
                nn.Dropout(),
                nn.Linear(256 * 2 * 2, 4096),
                nn.ReLU(inplace=True),
                nn.Dropout(),
                nn.Linear(4096, 4096),
                nn.ReLU(inplace=True),
                nn.Linear(4096, num_classes),
            )

        def forward(self, x):
            x = self.features(x)
            x = self.avgpool(x)
            x = torch.flatten(x, 1)
            x = self.classifier(x)
            return x


    class GlebNet(nn.Module):
        def __init__(self, num_classes):
            super(GlebNet, self).__init__()
            self.name = "Mini AlexNet"
            self.features = nn.Sequential(
                nn.Conv2d(3, 64, kernel_size=3, stride=2, padding=1),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(kernel_size=2, stride=2),

                nn.Conv2d(64, 128, kernel_size=3, padding=1),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(kernel_size=2, stride=2),
            )

            self.avgpool = nn.AdaptiveAvgPool2d((4, 4))

            self.classifier = nn.Sequential(
                nn.Dropout(),
                nn.Linear(128 * 4 * 4, num_classes),
            )

        def forward(self, x):
            x = self.features(x)
            x = self.avgpool(x)
            x = torch.flatten(x, 1)
            x = self.classifier(x)
            return x


    def train_one_epoch(net, optimizer, criterion):
        print(f"start training {net.name}")
        net.train()  # Set the model to evaluation mode
        start = time.time()
        for i, data in enumerate(trainloader, 0):
            # get the inputs; data is a list of [inputs, labels]
            inputs, labels = data
            inputs, labels = inputs.to(device, dtype=torch.float32, non_blocking=True), labels.to(device,non_blocking=True)

            # zero the parameter gradients
            optimizer.zero_grad()

            # forward + backward + optimize
            outputs = net(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
        end = time.time() - start
        print(GREEN + net.name + RESET, "one epoch training have ended in", str(time.time() - start) + "s")
        # PATH = f'./data/{net.name}.pth'
        # torch.save(net.state_dict(), PATH)
        return end


    def test_model(net, epoch):
        net.eval()  # Set the model to evaluation mode
        correct = 0
        total = 0
        # since we're not training, we don't need to calculate the gradients for our outputs
        with torch.no_grad():
            for data in testloader:
                images, labels = data
                images, labels = images.to(device, dtype=torch.float32), labels.to(device)

                # calculate outputs by running images through the network
                outputs = net(images)
                # the class with the highest energy is what we choose as prediction
                _, predicted = torch.max(outputs.data, 1)
                total += labels.size(0)
                correct += (predicted == labels).sum().item()

        print(f"Accuracy of the {net.name} on the {epoch}'th epoch is {100 * correct // total}%")
        return (100 * correct // total)


    def draw_plot(vals, colors, names, times, n):
        print(vals)
        epochs = np.arange(0, n)
        plt.figure(figsize=(n, 5))
        bar_width = 0.2
        bar_positions = np.arange(len(epochs))
        for i, (data, color, name) in enumerate(zip(vals, colors, names)):
            plt.bar(
                bar_positions + i * bar_width,
                data,
                width=bar_width,
                label=name,
                color=color,
                edgecolor='black',
                linewidth=1
            )
        plt.xlabel('Epochs', fontsize=5 + n // 4)
        plt.ylabel('Accuracy', fontsize=5 + n // 4)
        plt.title('Accuracy for Each Network over Epochs', fontsize=5 + n // 4)
        plt.legend(fontsize=5 + n // 4)
        plt.xticks(bar_positions + (len(vals) - 1) * bar_width / 2, epochs)
        plt.grid(True)
        # plt.show()
        plt.savefig("epoch_accuracy_model.png")
        plt.clf()

        plt.figure(figsize=(5, 5))
        plt.bar(names, times, color=colors, edgecolor='black', linewidth=1)
        plt.xlabel('Networks', fontsize=14)
        plt.ylabel('Time', fontsize=14)
        plt.title('Time for Each Network', fontsize=16)
        plt.grid(True)
        plt.savefig("epoch_time_model.png")
        plt.clf()


    def compare_models():
        models = [Net().to(device), ImprovedNet().to(device), AlexNetSmallInput(10).to(device), GlebNet(10).to(device)]
        criterion = nn.CrossEntropyLoss()
        optims = {}
        accuracies = []
        colors = [
            (0, 1, 0),
            (0, 0, 1),
            (1, 0, 0),
            (1, 0, 1)
        ]
        names = []
        for idx, model in enumerate(models):
            optims[model.name] = optim.Adam(model.parameters(), lr=0.001)
            accuracies.append(np.array([]))
            names.append(model.name)

        epochs_nums = 0
        while(1):
            times = []
            for idx, model in enumerate(models):
                time = train_one_epoch(model, optims[model.name], criterion)
                acc = test_model(model, epochs_nums)
                accuracies[idx] = np.append(accuracies[idx], np.asarray([acc]))
                times.append(time)
            epochs_nums += 1
            draw_plot(accuracies, colors, names, times, epochs_nums)

    compare_models()