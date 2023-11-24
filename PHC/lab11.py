import numpy as np
import torch
import torch.nn as nn

def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x):
    return x * (1 - x)

def loss(t, y):
    return t - y

class Layer:
    def __init__(self, in_size, out_size, func):
        # self.fc1 = nn.Linear(in_size, out_size)  # Полносвязный слой 1
        self.func = nn.ReLU()  # Функция активации

    def activate(self, x):
        return self.func(x)

    def backward(self, x):
        return sigmoid_derivative(x)

class Model:
    def __init__(self, input_size=2, hidden_size=2, output_size=1, learning_rate=0.1):
        self.hidden_neuron1 = Neuron(input_size)
        self.hidden_neuron2 = Neuron(input_size)
        self.output_neuron = Neuron(hidden_size)
        self.output_layer_res = 0
        self.hidden_layer_res = 0
        self.lr = learning_rate

    def forward(self, inputs):
        # Forward Propagation
        hidden_layer_output1 = self.hidden_neuron1.activate(inputs)
        hidden_layer_output2 = self.hidden_neuron2.activate(inputs)
        self.hidden_layer_res = np.array([hidden_layer_output1, hidden_layer_output2])
        # print(inputs, inputs.shape, hidden_layer_output1, hidden_layer_output1.shape, self.hidden_layer_res.shape)
        self.output_layer_res = self.output_neuron.activate(self.hidden_layer_res)
        # print(self.output_layer_res)
        return self.hidden_layer_res, self.output_layer_res

    def backward(self, inputs, err):
        # Backpropagation
        d_output = err * self.output_neuron.backward(self.output_layer_res)

        error_hidden1 = d_output * self.output_neuron.weight[0]
        error_hidden2 = d_output * self.output_neuron.weight[1]

        d_hidden1 = error_hidden1 * self.hidden_neuron1.backward(self.hidden_layer_res[0])
        d_hidden2 = error_hidden2 * self.hidden_neuron2.backward(self.hidden_layer_res[1])

        # Update weights and biases
        self.output_neuron.weight += self.hidden_layer_res * d_output * self.lr
        self.output_neuron.bias += np.sum(d_output) * self.lr

        self.hidden_neuron1.weight += inputs * d_hidden1 * self.lr
        self.hidden_neuron1.bias += np.sum(d_hidden1) * self.lr

        self.hidden_neuron2.weight += inputs * d_hidden2 * self.lr
        self.hidden_neuron2.bias += np.sum(d_hidden2) * self.lr

inputLayerNeurons, hiddenLayerNeurons, outputLayerNeurons = 2, 2, 1
model_2 = Model(inputLayerNeurons, hiddenLayerNeurons, outputLayerNeurons, 0.1)

inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
expected_output = np.array([[0], [1], [1], [0]])

epochs = 10000

for epoch in range(epochs):
    for i in range(len(inputs)):
        hidden_res, output_res = model_2.forward(inputs[i])
        err = loss(expected_output[i], output_res)
        model_2.backward(inputs[i], err)

for i in range(len(inputs)):
    _, output = model_2.forward(inputs[i])
    print(f"Input: {inputs[i]}, Predicted Output: {output} -> {round(output)}")