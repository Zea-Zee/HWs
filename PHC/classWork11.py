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


dataset = GeneratorDataset(64, 10, 12800)
dataloader = data.DataLoader(dataset, batch_size=16)
for x, y in dataloader:
    break
print(x.shape, y.shape)

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
model_seq = SimpleModelSeq(64, 10)
print(list(model_seq.parameters()))

opt = torch.optim.Adam(model_seq.parameters(), lr=0.001)
for x, y in dataloader:
    opt.zero_grad()
    y_pred = model_seq(x)
    loss = mae(y, y_pred)
    loss.backward()
    # print('Loss', loss.item())
    opt.step()
print('Loss', loss.item())