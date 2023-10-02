import numpy as np

def floating_average(vector, window):
    cumsum = np.cumsum(vector)
    cumsum[window:] = cumsum[window:] - cumsum[:-window]
    return cumsum[window - 1:] / window

vector = [10, 20, 15, 15, 12, 12, 16, 9, 26, 12]
window = 2

result = floating_average(vector, window)
print(result)
