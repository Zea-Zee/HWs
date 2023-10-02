# import argparse
# import time
RED = '\033[91m'
RESET = '\033[0m'


def fill_matrix(m_list, lines):
    """Takes list where will be the matrix and fill it by data from list of input matrix strings.
    When string is \n it returns """
    line = lines.pop(0)
    while line:
        m_list.append(list())
        for el in line.split(' '):
            if el == '\n':
                return
            m_list[-1].append(int(el))
        # print(m_list[-1])
        if len(m_list) > 1 and len(m_list[-1]) < len(m_list[-2]):
            raise ValueError(RED + "Incorrect number of columns in matrix" + RESET)
        if not lines:
            return
        line = lines.pop(0)


def sum_two_matrices(matrix_a, matrix_b):
    if not matrix_a or not matrix_b:
        raise ValueError(RED + "One of arguments isn't list with matrix" + RESET)
    if len(matrix_a) != len(matrix_b):
        raise ValueError(RED + "Matrices have different sizes" + RESET)
    if len(matrix_a[0]) != len(matrix_b[0]):
        raise ValueError(RED + "Matrices have different sizes" + RESET)
    res_matrix = list()
    for i in range(len(matrix_a)):
        res_matrix.append(list())
        for j in range(len(matrix_a[i])):
            res_matrix[i].append(matrix_a[i][j] + matrix_b[i][j])
    return res_matrix


def sub_two_matrices(matrix_a, matrix_b):
    if not matrix_a or not matrix_b:
        raise ValueError(RED + "One of arguments isn't list with matrix" + RESET)
    if len(matrix_a) != len(matrix_b):
        raise ValueError(RED + "Matrices have different sizes" + RESET)
    if len(matrix_a[0]) != len(matrix_b[0]):
        raise ValueError(RED + "Matrices have different sizes" + RESET)
    res_matrix = list()
    for i in range(len(matrix_a)):
        res_matrix.append(list())
        for j in range(len(matrix_a[i])):
            res_matrix[i].append(matrix_a[i][j] - matrix_b[i][j])
    return res_matrix


def print_matrix(matrix, msg=""):
    if msg:
        print(msg)
    else:
        print("printed matrix:")
    for i in range(len(matrix)):
        # for j in range(len(matrix[i])):
        #     print(str(matrix[i][j]) + " ", end='')
        # print("\n")
        print(matrix[i])


class Pupa:
    def __init__(self, name="Pupa", salaries_num=0):
        self.salaries_num = salaries_num
        self.name = name
        self.work = sum_two_matrices

    def take_salary(self):
        self.salaries_num += 1

    # @staticmethod
    def do_work(self, filename_a, filename_b):
        a = open(filename_a, 'r')
        b = open(filename_b, 'r')
        matrix_a = list()
        matrix_b = list()
        fill_matrix(matrix_a, a.readlines())
        fill_matrix(matrix_b, b.readlines())
        # res_matrix = sum_two_matrices(matrix_a, matrix_b)
        res_matrix = self.work(matrix_a, matrix_b)
        print_matrix(res_matrix, self.name + "'s result")


class Lupa(Pupa):
    def __init__(self, name="Lupa", total_profit=0):
        # self.total_profit = total_profit
        # self.name = name
        super().__init__()
        self.work = sub_two_matrices
        self.name = name

    # def take_salary(self, sum):
    #     self.total_profit += sum


class Accountant:
    def __init__(self, name="Greedy"):
        self.name = name

    @staticmethod
    def give_salary(worker):
        worker.take_salary()


pupa = Pupa()
lupa = Lupa()
greedy = Accountant()
greedy.give_salary(pupa)
greedy.give_salary(lupa)
pupa.do_work("input.txt", "input.txt")
lupa.do_work("input.txt", "input.txt")
