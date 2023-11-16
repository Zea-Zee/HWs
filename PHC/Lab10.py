import numpy as np

class Item:
    def __init__(self, count=3, max_count=16):
        self._count = count
        self._max_count = max_count

    def update_count(self, val):
        if val <= self._max_count:
            self._count = val
            return True
        else:
            return False

    # Свойство объекта. Не принимает параметров кроме self, вызывается без круглых скобок
    # Определяется с помощью декоратора property
    @property
    def count(self):
        return self._count

    # Ещё один способ изменить атрибут класса
    @count.setter
    def count(self, val):
        self._count = val
        if val <= self._max_count:
            self._counts = val
        else:
            pass

inputLayerNeurons, hiddenLayerNeurons, outputLayerNeurons = 2, 2, 1
hidden_weights = np.random.uniform(size=(inputLayerNeurons, hiddenLayerNeurons))
hidden_bias = np.random.uniform(size=(1,hiddenLayerNeurons))
output_weights = np.random.uniform(size=(hiddenLayerNeurons,outputLayerNeurons))
output_bias = np.random.uniform(size=(1,outputLayerNeurons))

def sigmoid (x):
    return 1/(1 + np.exp(-x))

hidden_layer_activation = np.dot(inputs,hidden_weights)
hidden_layer_activation += hidden_bias
hidden_layer_output = sigmoid(hidden_layer_activation)
output_layer_activation = np.dot(hidden_layer_output,output_weights)
output_layer_activation += output_bias
predicted_output = sigmoid(output_layer_activation)