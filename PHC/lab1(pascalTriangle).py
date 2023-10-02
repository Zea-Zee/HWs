import argparse
import math

parser = argparse.ArgumentParser()
parser.add_argument("height")
args = parser.parse_args()
h = int(args.height)
# print(h)

matrix = [[0 for x in range(h)] for y in range(h)]
matrix[0][0] = 1
# print(matrix)
for i in range(1, h):
    for j in range(0, h):
        if j == 0:
            matrix[i][j] = matrix[i - 1][j]
        else:
            matrix[i][j] = matrix[i - 1][j - 1] + matrix[i - 1][j]
        # matrix[i][j] = matrix[i - 1][j] + matrix[i - 1][j + 1]

num_of_last = 0
max_el_len = 0
for el in matrix[h - 1]:
    num_of_last += 1
    max_el_len = max(max_el_len, len(str(el)))
len_of_last = num_of_last * (max_el_len + 1)
for i in range(0, h):
    len_of_cur = 0
    for el in matrix[i]:
        if el == 0:
            continue
        len_of_cur += 1 + max_el_len# len(str(el))
    # print((h - i) // 2)
    print(math.ceil((len_of_last - len_of_cur) / 2) * ' ', end='')
    for el in matrix[i]:
        if el != 0:
            print("{:^{precision}d}".format(el, precision=max_el_len), end=' ')
            # f"""sdfsdf{huesos}sfsefsefse"""
    print('')
# print(matrix)