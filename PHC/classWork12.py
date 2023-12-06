import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import time

BLUE = '\033[94m'
CYAN = '\033[96m'
GREEN = '\033[92m'
RED = '\033[91m'
RESET = '\033[0m'


def im2col(img, size):
    h, w = img.shape
    rows, cols = size[0] * size[1], (w - size[1] + 1) * (h - size[0] + 1)
    res = np.zeros((rows, cols))
    # print(rows, cols, "\n", res)
    idx, jdx = 0, 0
    for i in range(cols):
        if jdx + size[1] > w:
            idx += 1
            jdx = 0
        res[:, i] = img[idx:idx + size[0], jdx:jdx + size[1]].flatten()
        # print("2d sub\n", sub, "\n___")
        # print("1d sub\n", sub, "\n___")
        jdx += 1
    return res


def im2col_memory_trick(image, kernel_size):
    kh, kw = kernel_size
    col_height = image.shape[0] - kh + 1
    col_width = image.shape[1] - kw + 1
    shape = (col_height, col_width, kh, kw)
    strides = (image.strides[0], image.strides[1], image.strides[0], image.strides[1])
    columns = np.lib.stride_tricks.as_strided(image, shape=shape, strides=strides)
    columns = columns.reshape(kh * kw, -1)
    return columns


def convolution(img, kernel, res_shape):
    h, w, c = img.shape
    res = np.empty(res_shape, dtype=np.uint8)
    # print("image")
    # print(img)
    # print("------------------------")
    for i in range(c):
        # print("chan")
        # print(img[:, :, i])
        # col = im2col(img[:, :, i], kernel.shape)
        col = im2col(img[:, :, i], kernel.shape)
        # print("res")
        res[:, :, i] = np.dot(kernel.flatten(), col).reshape(res[:, :, i].shape)
        # print(res[:, :, i])
    # result = np.matmul(kernel.flatten(), im2col_result)
    # result = np.dot(kernel.flatten(), im2col_result).reshape(im2col_result.shape[1], -1)
    return res


# image = np.random.randint(0, 100, (6, 4, 3))
image = np.random.randint(0, 100, (600, 400, 3))
kernel = np.asarray([[1, 0, -1], [2, 0, -2], [1, 0, -1]])
ih, iw, c = image.shape
kh, kw = kernel.shape
res_shape = (ih - kh + 1, iw - kw + 1, c)
start = time.time_ns()
convolution_result = convolution(image, kernel, res_shape)
print("my conv have taken", BLUE, (time.time_ns() - start) / 1000000, "ms", RESET)
print(convolution_result)

image_tensor = torch.tensor(image, dtype=torch.float32).permute(2, 0, 1).unsqueeze(0)
sobel_kernel_tensor = torch.tensor(kernel, dtype=torch.float32).unsqueeze(0).unsqueeze(0)
start = time.time_ns()
result1 = F.conv2d(image_tensor[0:1, 0:1], sobel_kernel_tensor, padding=0)
result2 = F.conv2d(image_tensor[0:1, 1:2], sobel_kernel_tensor, padding=0)
result3 = F.conv2d(image_tensor[0:1, 2:3], sobel_kernel_tensor, padding=0)
result = torch.cat([result1, result2, result3], dim=1)
result_np = result.squeeze().permute(1, 2, 0).detach().numpy()
print("NN conv have taken", BLUE, (time.time_ns() - start) / 1000000, "ms", RESET)
print(result_np)