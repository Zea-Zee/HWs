import numpy as np
import torch


def get_start_indices(i, ih, iw, h, w):
    if i > iw - w:
        return i + 1, 0

def im2col(img, size):
    h, w = img.shape
    rows, cols = size[0] * size[1], (w - size[0] + 1) * (h - size[1] + 1)
    print(size, w, h)
    print(img)
    res = np.eye(rows, cols)
    for i in range(cols):
        for j in range(rows):
            start_row, end_row = i // (w - size[0] + 1), i // (w - size[0] + 1) + size[0]
            start_col, end_col = 
            res[j][i] = img[j // 3 + idx][j % 3 + jdx]
    print(res)

img = np.random.randint(0, 255, (4, 4))
img = np.array([[0, 1, 2, 3], [4, 5, 6, 7], [8, 9, 10, 11], [12, 13, 14, 15]])
im2col(img, (3, 3))