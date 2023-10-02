import numpy as np
from collections import Counter


def sort_arr_by_freq(arr):
    uniqs, freq = np.unique(arr, return_counts=True)  # get uniq elements and their counts
    # print(freq)
    sorted_indices = np.argsort(-freq)  # sort indices by their freq in not increasing order
    # print(sorted_indices)
    sorted_vals = uniqs[sorted_indices]  # reorder qunique elements by their indices
    sorted_freqs = freq[sorted_indices]  # reorder freqa by indices
    # print(sorted_freqs)
    # print(sorted_vals)
    arr = np.repeat(sorted_vals, sorted_freqs)
    return arr


def count_uniq_pixeles(picture):
    # print(np.unique(picture, return_counts=True, axis=0))
    # pixeles, freqs = np.unique(picture, return_counts=True, axis=0)
    # # print(picture)
    # uniqs = 0
    # for freq in freqs:
    #     if freq == 1:
    #         uniqs += 1
    return len(np.unique(picture, axis=0))


def compute_moving_avarage(arr, window):
    # if window_len == 0:
    #     return [0] * len(arr)
    # sum = np.cumsum(arr)
    # for i in range(len(sum)):
    #     sum[i] /= (i + 1)
    # sumc = np.copy(sum)
    # print(sum)
    # for i in range(1, len(sum)):
    #     if i >= window_len:
    #         sumc[i] -= sum[i - window_len]
    # return sumc
    if window == 0:
        return [0] * len(arr)
    # if window == 1:
    #     return arr
    res = np.cumsum(arr)
    res[window:] -= res[:-window]
    # res[window - 1:] /= window
    return res[window - 1:] / window


def is_triangle(arr):
    a, b, c = arr[:, 0], arr[:, 1], arr[:, 2]
    return arr[np.logical_and(a + b > c, a + c > b, b + c > a)]


arr = np.array([1,2,2,3,3,3,4])
# print(arr)
print(sort_arr_by_freq(arr))
# print(arr, "\n\n\n2nd task")
# print(np.repeat([1,2,3], [1,2,3]))
picture = np.array([[0,0,0],[255,255,255],[0,255,0],[0,255,0],[0,255,0],[0,255,0]])
# picture = tuple(np.array([1,2,3], ).tolist())
print(count_uniq_pixeles(picture))
# np.repeat(picture, [1,2,3,4,5,6])
# print(np.unique(picture, return_counts=True, axis=0))
# print("number of unique pixels is", count_uniq_pixeles(picture))
arr = np.array([155, 163, 167, 131, 158, 147, 130, 145, 128])
# print(arr)
print(compute_moving_avarage(arr, 1))
print(is_triangle(np.array(([3, 4, 5], [1, 2, 4], [1, 2, 3], [2,2,2]))))