import argparse
import random
import math

parser = argparse.ArgumentParser()
parser.add_argument("len")#, help = "length of the list")
args = parser.parse_args()
len = int(args.len)
arr = list([1] * len)
for i in range(0, len):
    arr[i] = math.floor(random.random() * len)  #for more detailed example numbers are in [0;N] not in [0;1]
    # print(el)

def bubble_sort(arr, len):
    for j in range(0, len - 1):
        for i in range(0, len - 1):
            # print(arr)
            if arr[i] > arr[i + 1]:
                arr[i], arr[i + 1] = arr[i + 1], arr[i]
                # tmp = arr[i]
                # arr[i] = arr[i + 1]
                # arr[i + 1] = tmp
            # print(arr, "next")
# print(arr)
bubble_sort(arr, len)
print(arr)