import numpy as np
from collections import Counter

def sort_arr_by_freq(arr):
    uniqs, freq = np.unique(arr, return_counts=True)
    print(freq)
    sorted_indices = np.argsort(-freq)
    print(sorted_indices)
    sorted_vals = uniqs[sorted_indices]
    sorted_freqs = freq[sorted_indices]
    print(sorted_freqs)
    print(sorted_vals)
    arr = np.repeat(sorted_vals, sorted_freqs)
    return arr
    # print(res)
    # arr = res

def count_uniq_pixeles(picture):
    pixeles, freqs = np.unique(picture, return_counts=True, axis=0)
    uniqs = 0
    for freq in freqs:
        if freq == 1:
            uniqs += 1
    return uniqs

def compute_moving_avarage(arr, window_len):
    if window_len == 0:
        return [0] * len(arr)
    sum = np.cumsum(arr)
    for i in range(len(sum)):
        sum[i] /= (i + 1)
    sumc = np.copy(sum)
    print(sum)
    for i in range(1, len(sum)):
        if i >= window_len:
            sumc[i] -= sum[i - window_len]
    return sumc

arr = np.array([1,2,2,3,3,3,4])
print(arr)
arr = sort_arr_by_freq(arr)
print(arr, "\n\n\n2nd task")
# print(np.repeat([1,2,3], [1,2,3]))
picture = np.array([[0,0,0],[255,255,255],[0,255,0],[0,255,0],[0,255,0],[0,255,0]])
# np.repeat(picture, [1,2,3,4,5,6])
# print(np.unique(picture, return_counts=True, axis=0))
print("number of unique pixels is", count_uniq_pixeles(picture))
arr = np.arange(0, 1100, 100)
print(arr)
print(compute_moving_avarage(arr, 2))