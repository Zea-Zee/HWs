import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import torch.utils.data as data


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


dataset = GeneratorDataset(64, 10, 128)
dataloader = data.DataLoader(dataset, batch_size=16)
for x, y in dataloader:
    break
print(x.shape, y.shape)


class SimpleModel(nn.Module):
    def __init__(self, in_ch, out_ch):
        super().__init__()

        self.model = nn.Sequential(
            nn.Linear(in_ch, 32),
            nn.ReLU(),
            nn.Linear(32, out_ch, bias=False)
        )

    def forward(self, x):
        return self.model(x)


mae = nn.L1Loss()

def loss_batch(model, loss_func, xb, yb, opt=None):
    loss = loss_func(model(xb), yb)

    if opt is not None:
        loss.backward()
        opt.step()
        opt.zero_grad()

    return loss.item(), len(xb)

def fit(epochs, model, loss_func, opt, data):
    for epoch in range(epochs):
        model.train()
        for x, y in data:
            for xb, yb in x:
                loss_batch(model, loss_func, xb, yb, opt)

            model.eval()
            with torch.no_grad():
                losses, nums = zip(
                    *[loss_batch(model, loss_func, xb, yb) for xb, yb in y]
                )
            val_loss = np.sum(np.multiply(losses, nums)) / np.sum(nums)

            print(epoch, val_loss)

model = SimpleModel(64, 10)
opt = optim.SGD(model.parameters(), lr=0.001, momentum=0.9)
print(model, end="\n------------------\n")
print(dataloader)
fit(1000, model, mae, opt, dataloader)